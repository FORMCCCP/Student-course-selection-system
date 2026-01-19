module;
#include "pqxx/pqxx"


export module secretarybroker;
import std;
import broker;
import domain;
import coursebroker;

using std::string;
using std::print;

export class Secretarybroker : public Broker{
public:
    Secretarybroker(std::shared_ptr<pqxx::connection> conn);

    class Secretary* returnSecretary(string id);           //返回秘书对象
    bool handleLogin(string id, string password);          //处理登陆

    bool studentExistYesOrNO(string s_id);                 //学生对象是否存在
    void addtheStudent(string s_id);                       //增加学生
    void deletetheStudent(string s_id);                    //删除学生

    bool teacherExistYesOrNO(string t_id);                 //教师对象是否存在
    void addtheteacher(string t_id);                       //增加教师
    void deletetheTeacher(string t_id);                    //删除教师

    bool courseExistYesOrNO(string c_id);                  //课程对象是否存在
    void addtheCourse(string c_id);                        //增加课程
    void dedeletheCourse(string c_id);                     //删除课程

    void showCourseAndTeacher();                           //查看课程的教师
    void changeCT(Coursebroker& c);                                       //修改课程的教师
};

Secretarybroker::Secretarybroker(std::shared_ptr<pqxx::connection> conn):
    Broker(conn){}

//返回秘书对象
class Secretary* Secretarybroker::returnSecretary(string id){
    pqxx::result result = query(
                R"(SELECT id, name, password, secondaryPassword  FROM secretarys
                    where id=$1)",id);
    string se_id = result[0][0].as<string>();
    string se_name = result[0][1].as<string>();
    string se_password = result[0][2].as<string>();
    string se_sepassword = result[0][3].as<string>();

    class Secretary* sec_ptr = new Secretary(se_id,se_name,se_password,se_sepassword);
    return sec_ptr;
}

//处理登陆
bool Secretarybroker::handleLogin(string id, string password){
    pqxx::result result_id = query(
                R"(SELECT * FROM secretarys
                    WHERE id=$1)",id
                                );
    pqxx::result result_pw = query(
                R"(SELECT * FROM secretarys
                    WHERE id=$1 AND password=$2 )",id,password);

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

//学生对象是否存在
bool Secretarybroker::studentExistYesOrNO(string s_id){
    pqxx::result result = query(
                R"(SELECT * FROM students WHERE id=$1)",s_id);
    if(result.empty()){
        return false;
    }else return true;
}

//增加学生
void Secretarybroker::addtheStudent(string s_id){
    string s_name,s_password,s_major,s_grade;
    int session;
    print("输入学生姓名:");
    std::cin>>s_name;
    print("设置密码:");
    std::cin>>s_password;
    print("选择专业:");
    std::cin>>s_major;
    print("设置年级(大一、大二、大三、大四):");
    while(1){
        std::cin>>s_grade;
        if(s_grade == "大一" || s_grade == "大二" || s_grade == "大三" || s_grade == "大四"){
            break;
        }else{
            print("错误输入，请重新设置年级:");
            continue;
        }
    }
    print("设置学期(1、2):");
    while(1){
        std::cin>>session;
        if(session == 1 || session == 2){
            break;
        }else{
            print("错误输入，请重新设置学期:");
            continue;
        }
    }
    execute("INSERT INTO students VALUES($1, $2, $3, $4, 0, $5, $6)",
            s_id, s_name,s_password,s_major,s_grade,session);
    return;
}

//删除学生
void Secretarybroker::deletetheStudent(string s_id){
    try{
    execute("UPDATE courses SET current_capacity = current_capacity-1 WHERE id IN "
            "(SELECT course_id FROM enrollments WHERE student_id=$1)",s_id);
    execute("DELETE FROM enrollments WHERE student_id=$1",s_id);
    execute("DELETE FROM students WHERE id = $1",s_id);
    }catch (const std::exception& e) {
        std::cerr << "删除失败 " << e.what() << std::endl;
        throw;
    }
    print("删除成功\n");
}

//教师对象是否存在
bool Secretarybroker::teacherExistYesOrNO(string t_id){
    pqxx::result result = query(
                R"(SELECT * FROM teachers WHERE id=$1)",t_id);
    if(result.empty()){
        return false;
    }else return true;
}

//增加教师
void Secretarybroker::addtheteacher(string t_id){
    string t_name, t_password, t_major;
    print("输入教师名字:");
    std::cin>>t_name;
    print("设置密码:");
    std::cin>>t_password;
    print("设置教学专业:");
    std::cin>>t_major;
    execute("INSERT INTO teachers VALUES($1, $2, $3, $4)",
            t_id, t_name,t_password,t_major);
    print("添加成功\n");
}

//删除教师
void Secretarybroker::deletetheTeacher(string t_id){
        execute("UPDATE courses SET teacher_id = 't0000' WHERE teacher_id=$1",t_id);
        execute("DELETE FROM teachers WHERE id=$1",t_id);
    print("删除成功\n");
}

//课程对象是否存在
bool Secretarybroker::courseExistYesOrNO(string c_id){
    pqxx::result result = query(
                R"(SELECT * FROM courses WHERE id=$1)",c_id);
    if(result.empty()){
        return false;
    }else return true;
}

//增加课程
void Secretarybroker::addtheCourse(string c_id){
    string c_name, c_credits, c_capacity, c_grade, c_major;
    int c_session;
    print("输入课程名(唯一):");
    while(1){
        std::cin>>c_name;
        pqxx::result result = query(
                    R"(SELECT name FROM courses WHERE name=$1)",c_name);
        if(!result.empty()){
            print("该课程已存在!\n");
            continue;
        }else {
            break;
        }
    }
    print("设置学分:");
    std::cin>>c_credits;
    print("设置容量:");
    std::cin>>c_capacity;
    print("设置开放年级(大一、大二、大三、大四):");
    while(1){
        std::cin>>c_grade;
        if(c_grade == "大一" || c_grade == "大二" || c_grade == "大三" || c_grade == "大四"){
            break;
        }else{
            print("错误输入，请重新设置年级:");
            continue;
        }
    }
    print("设置开放学期(1、2):");
    while(1){
        std::cin>>c_session;
        if(c_session == 1 || c_session == 2){
            break;
        }else{
            print("错误输入，请重新设置学期:");
            continue;
        }
    }
    print("设置开设专业:");
    std::cin>>c_major;
    execute("INSERT INTO courses VALUES($1,$2,$3,0 ,‘t0000’,$4,$5,$6,$7)",
            c_id,c_name,c_credits,c_capacity,c_session,c_grade,c_major);
    print("添加成功\n");
}

//删除课程
void Secretarybroker::dedeletheCourse(string c_id){
    execute("DELETE FROM enrollments WHERE course_id=$1",c_id);
    execute("DELETE FROM courses WHERE id=$1",c_id);
    print("删除成功\n");
}

void Secretarybroker::showCourseAndTeacher(){
    pqxx::result result = query("SELECT id,name, teacher_id, majored FROM courses");
    for(int i = 0;i<result.size();++i){
        string c_id =result[i][0].as<string>();
        string c_name =result[i][1].as<string>();
        string t_id =result[i][2].as<string>();
        string major =result[i][3].as<string>();
        print("{} : {}\n",c_name,c_id);
        print("所属专业: {}\n",major);
        if(t_id == "t0000"){
            print("教师:未分配\n",t_id);
        }else {
            print("教师:{}\n",t_id);
        }
        print("\n");
    }
}

//修改课程的老师
void Secretarybroker::changeCT(Coursebroker& c){
    string c_id;
    while(1){
        print("输入修改的课程:");
        std::cin>>c_id;
        pqxx::result result = query("SELECT * FROM courses WHERE id=$1",c_id);
        if(result.empty()){
            print("不存在课程\n");
            continue;
        }else{
            break;
        }
    }
    string t_id;
    while(1){
        print("输入修改后的老师ID:");
        std::cin>>t_id;
        pqxx::result result = query("SELECT majored FROM teachers WHERE id=$1",t_id);
        if(result.empty()){
            print("不存在老师\n");
            continue;
        }else{
            string t_m = result[0][0].as<string>();
            string c_m = c.returnCourseMajor(c_id);
            if(t_m != c_m){
            print("非同一专业\n");
            return;
            }else{
                break;
                }
        }
    }
    execute("UPDATE courses SET teacher_id=$1 WHERE id=$2",t_id,c_id);
}
