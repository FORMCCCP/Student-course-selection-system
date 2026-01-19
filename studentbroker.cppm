//数据层：学生层
//从数据库提取学生相关数据
module;
#include "pqxx/pqxx"


export module studentbroker;
import std;
import broker;
import coursebroker;
import domain;

using std::string;

export class Studentbroker: public Broker{
public:
    Studentbroker(std::shared_ptr<pqxx::connection> conn);                                      //构造函数
    bool handleLogin(string id, string password);                                               //验证账号密码
    class Student* returnStudent(string id);                                                    //返回学生对象
    pqxx::result returnAvailableCoursesid(string id);                                           //返回可选择课程的结果
    string returnStudentName(string s_id);                                                             //返回学生名字
    std::vector<class Course*> checkAvailableCourse(const string s_id,Coursebroker& Coubroker); //获取可选择课程
    void chooseCourseOperation(string c_id, string s_id, Coursebroker& Coubroker);              //选择课程的操作
    void exitCourseOperatrion(string c_id, string s_id, Coursebroker& Coubroker);               //退出课程的操作
    int calculateTotalGrade(string s_id, Coursebroker& Coubroker);                              //计算总成绩
};

Studentbroker::Studentbroker(std::shared_ptr<pqxx::connection> conn):
    Broker(conn){}

//验证账号和密码
bool Studentbroker::handleLogin(string id, string password){
    pqxx::result result_id = query(
                R"(SELECT * FROM students
                    where id=$1)",id
                                );
    pqxx::result result_pw = query(
                R"(SELECT * FROM students
                    where id=$1 AND password=$2 )",id,password);

    if(result_id.empty()){
        std::print("\n不存在该ID\n");
        return false;
    }

    if(!result_pw.empty()){
        std::cout << "\033[2J\033[H" << std::flush;
        return true;
    }else{
        std::print("\n密码错误\n");
        return false;
        }
}

//返回学生名字
string Studentbroker::returnStudentName(string s_id){
    class Student* student = returnStudent(s_id);
    string name = student->getName();
    delete student;
    return name;
}

//返回学生对象
class Student* Studentbroker::returnStudent(string id){
    pqxx::result result = query(
                R"(SELECT id, name, password, major, total_credits,
                   grade, current_session  FROM students
                    where id=$1)",id
                                );
    string _id = result[0][0].as<string>();                         //姓名
    string name = result[0][1].as<string>();                        //ID
    string password = result[0][2].as<string>();                    //密码
    string major = result[0][3].as<string>();                       //专业
    int s_credits = result[0][4].as<int>();                         //学分
    string grade =result[0][5].as<string>();                        //年级
    int current_session = result[0][6].as<int>();                   //当前学期

    class Student* stu_ptr = new Student(name,_id,password,major,
                                         s_credits,current_session,grade);
    return stu_ptr;
}


//返回可选择课程的结果
pqxx::result Studentbroker::returnAvailableCoursesid(string id){
    pqxx::result result = query(
                R"(SELECT major, grade, current_session
                   FROM students WHERE id=$1 )",id);
    string m = result[0][0].as<string>();
    string g = result[0][1].as<string>();
    int s = result[0][2].as<int>();
    pqxx::result result1 = query(
                R"(SELECT id
                   FROM courses c
                   WHERE c.session=$1 AND c.grade=$2 AND c.majored=$3 AND c.current_capacity < capacity
                                                        AND c.id NOT IN(
                                                        SELECT course_id
                                                        FROM enrollments
                                                        WHERE student_id=$4
                                                            ) )",s,g,m,id);
    return result1;
}

//获取可选择课程
std::vector<class Course*> Studentbroker::checkAvailableCourse(const string s_id, Coursebroker& Coubroker){
    pqxx::result result = returnAvailableCoursesid(s_id);

    std::vector<string> availableID;
    size_t count = result.size();
    for(int i=0;i<count;++i){
        string str = result[i][0].as<string>();
        availableID.push_back(str);
    }
    std::vector<class Course*> availableCourses;
    for(string& s : availableID){
        class Course* cou_ptr = Coubroker.returnCourse(s);
        availableCourses.push_back(cou_ptr);
    }
    return availableCourses;
}

//选择课程的操作
void Studentbroker::chooseCourseOperation(string c_id, string s_id, Coursebroker& Coubroker){
    pqxx::result result = returnAvailableCoursesid(s_id);
    if(result.empty()){
        std::print("你没有可选择的课程\n");
        return;
    }
    for(int i =0;i<result.size();++i){
        string cid = result[i][0].as<string>();
        if(c_id == cid){
            execute("INSERT INTO enrollments VALUES($1,$2,$3)",s_id,c_id,-1);
            execute("UPDATE courses SET current_capacity = current_capacity + 1 WHERE id = $1", c_id);
            std::print("选取课程《{}》成功.\n\n",Coubroker.returnCourseName(c_id));
            return;
        }
    }
    std::print("输入ID错误,选课失败\n");
}


//退出课程的操作
void Studentbroker::exitCourseOperatrion(string c_id, string s_id, Coursebroker& Coubroker){
    pqxx::result result = query(
                R"(SELECT * FROM enrollments WHERE student_id=$1 AND course_id=$2 )",s_id,c_id);
    if(result.empty()){
        std::print("输入错误或你未选这门课程\n");
        return;
    }else{
        execute("DELETE FROM enrollments WHERE student_id=$1 AND course_id=$2",s_id,c_id);
        execute("UPDATE courses SET current_capacity = current_capacity - 1 WHERE id = $1", c_id);
        std::print("退出课程《{}》成功.\n\n",Coubroker.returnCourseName(c_id));
    }
}

//计算总成绩
int Studentbroker::calculateTotalGrade(string s_id, Coursebroker& Coubroker){
    pqxx::result result = query(
                R"(SELECT course_id, credit FROM enrollments WHERE student_id=$1)",s_id);
    if(result.size() == 0){
        return -1;
    }
    int totalgrade = 0;
    for(int i =0;i<result.size();++i){
        int g = result[i][1].as<int>();
        string cou = result[i][0].as<string>();
        if(g == -1){
            std::print("{}还未被评分\n",Coubroker.returnCourseName(cou));
        }else{
            std::print("{}的成绩:{}\n",Coubroker.returnCourseName(cou), g);
            totalgrade += g;
        }
    }
    execute("UPDATE students SET total_credits=$1 WHERE id=$2",totalgrade, s_id);
    return totalgrade;
}
