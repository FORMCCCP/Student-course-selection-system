module;
#include "pqxx/pqxx"

export module teacherbroker;

import broker;
import coursebroker;
import studentbroker;
import domain;
import std;
using std::string;

export class Teacherbroker: public Broker{
public:
    Teacherbroker(std::shared_ptr<pqxx::connection> conn);

    bool handleLogin(string id, string password);                                       //登陆处理
    class Teacher* returnTeacher(string t_id);                                          //返回教师对象
    pqxx::result returnTeachingCoursesid(string id);                                    //返回教授课程的结果
    std::vector<class Course*> obtainTeachingCourse(string id,Coursebroker& Coubroker); //获取教授课程
    void showCourseAndStudent(string c_id, string t_id,Studentbroker& Stubroker);       //展示某课程的学生及学分
    void evaluateGradeToStudent(string c_id);                 //为学生评分
};
Teacherbroker::Teacherbroker(std::shared_ptr<pqxx::connection> conn):
    Broker(conn){}


//验证账号和密码
bool Teacherbroker::handleLogin(string id, string password){
    pqxx::result result_id = query(
                R"(SELECT * FROM teachers
                    where id=$1)",id
                                );
    pqxx::result result_pw = query(
                R"(SELECT * FROM teachers
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

//返回教师对象
class Teacher* Teacherbroker::returnTeacher(string t_id){
    pqxx::result result = query(
                R"(SELECT id, name, password, majored FROM teachers
                    WHERE id=$1)",t_id);
    string tid = result[0][0].as<string>();
    string tname = result[0][1].as<string>();
    string tpassword = result[0][2].as<string>();
    string tmajored = result[0][3].as<string>();

    class Teacher* tea_str = new Teacher(tid, tname, tpassword, tmajored);
    return tea_str;
}


//返回教授课程的结果
pqxx::result Teacherbroker::returnTeachingCoursesid(string t_id){
    pqxx::result result = query(
                R"( SELECT id FROM courses WHERE teacher_id = $1)",t_id);
    return result;
}

//获取教授课程
std::vector<class Course*> Teacherbroker::obtainTeachingCourse(string t_id,Coursebroker& Coubroker){
    std::vector<class Course*> courses;
    pqxx::result result = returnTeachingCoursesid(t_id);
    if(result.empty()){
        return courses;
    }else{

        for(int i =0;i<result.size();++i){
            string str = result[i][0].as<string>();
            class Course* c = Coubroker.returnCourse(str);
            courses.push_back(c);
        }
        return courses;
    }
}

//展示某课程的学生及学分
void Teacherbroker::showCourseAndStudent(string c_id, string t_id,Studentbroker& Stubroker){
    pqxx::result result = query(
                R"(SELECT e.student_id, e.credit FROM enrollments e
                    WHERE  e.course_id IN (SELECT id FROM courses WHERE teacher_id = $1)
                    AND e.course_id = $2
                    )", t_id,c_id);
    if(result.empty()){
        std::print("无法找到该课程.");
        return;
    }
    std::vector<string> s_ids;
    std::vector<int> credits;
    if(result.size() ==0){
        std::print("该课程目前无学生\n");
        return;
    }
    for(int i=0;i<result.size();++i){
        string sid = result[i][0].as<string>();
        s_ids.push_back(sid);
        int cre = result[i][1].as<int>();
        credits.push_back(cre);
        if(cre != -1){
            print("{}  {}  {}\n",sid,Stubroker.returnStudentName(sid),cre);
        }else{
            print("{}  {}  暂未评分\n",sid,Stubroker.returnStudentName(sid));
        }
    }
    evaluateGradeToStudent(c_id);

}

//为学生评分
void Teacherbroker::evaluateGradeToStudent(string c_id){
    std::vector<string> sids;
    pqxx::result result = query(
            R"(SELECT student_id FROM enrollment WHERE course_id=$1)",c_id);
    for(int i=0; i<result.size();++i){
        string sid = result[i][0].as<string>();
        sids.push_back(sid);
        }
    while(1){
        string s_id;
        std::print("选择学生(输入ID):");
        std::cin >> s_id;
        int count = 0;
        for(string& s: sids){
            if(s == s_id){
                std::print("获得学生\n");
                break;
            }
            ++count;
        }
        if(count==sids.size()){
            std::print("不存在该学生，请重新输入\n");
            continue;
        }
        std::print("评分:");
        int c;
        std::cin >> c;
        if(c>=0 && c<=100){
            std::print("评分成功\n");
            execute("UPDATE enrollments SET credit = $1 WHERE id = $2",c,c_id);
            return;
        }else{
            std::print("无效分数\n");
            continue;
        }
    }
}
