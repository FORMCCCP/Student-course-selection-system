module;
#include "pqxx/pqxx"

export module ser.secretary;

import domain;
import secretarybroker;
import coursebroker;
import std;
using std::string;
using std::print;

export class SerSecretary{
private:
    Coursebroker coubroker;
    Secretarybroker secbroker;      //秘书数据层

    class Secretary* secretary;

public:
    SerSecretary(std::shared_ptr<pqxx::connection> conn);
    bool login();
    class Secretary* getSercretary();      //获取秘书对象
    bool verificationSecondaryPassword();   //二级密码验证

    void addStudent();              //增加学生
    void deleteStudent();           //删除学生
    void addTeacher();              //增加教师
    void deleteTeacher();           //删除教师
    void addCourse();               //增加课程
    void deleteCourse();            //删除课程

    void changeTeachersInCourse();  //修改课程教师
};

SerSecretary::SerSecretary(std::shared_ptr<pqxx::connection> conn):
    secbroker(conn),coubroker(conn){}

//获取秘书对象
class Secretary* SerSecretary::getSercretary(){
    return secretary;
}
//登陆
bool SerSecretary::login(){
    string id, password;
    print("输入ID:");
    std::cin>>id;
    print("输入密码:");
    std::cin>>password;
    if(secbroker.handleLogin(id, password)){
        secretary = secbroker.returnSecretary(id);
        return true;
    }else{
        return false;
    }
}
//二级密码验证
bool SerSecretary::verificationSecondaryPassword(){
    print("输入二级密码:");
    string password;
    std::cin>>password;
    if(password != secretary->getSecondary()){
        return false;
    }else return true;

}

//增加学生
void SerSecretary::addStudent(){
    print("输入要增加的学生ID:");
    string s_id;
    std::cin>>s_id;
    if(!secbroker.studentExistYesOrNO(s_id)){
        secbroker.addtheStudent(s_id);
        return;
    }else{
        print("该学生ID已存在\n");
        return;
    }
}

//删除学生
void SerSecretary::deleteStudent(){
    print("输入想要删除的学生ID:");
    string s_id;
    std::cin>>s_id;
    if(!secbroker.studentExistYesOrNO(s_id)){
        print("该学生不存在\n");
        return;
    }else{
        secbroker.deletetheStudent(s_id);
    }
}
//增加教师
void SerSecretary::addTeacher(){
    print("输入想要增加的教师ID:");
    string t_id;
    std::cin>>t_id;
    if(!secbroker.teacherExistYesOrNO(t_id)){
        secbroker.addtheteacher(t_id);
        return;
    }else{
        print("该教师ID已存在\n");
        return;
    }
}

//删除教师
void SerSecretary::deleteTeacher(){
    print("输入想要删除的教师ID:");
    string t_id;
    std::cin>>t_id;
    if(!secbroker.teacherExistYesOrNO(t_id)){
        print("该教师不存在");
        return;
    }else{
        secbroker.deletetheTeacher(t_id);
        return;
    }
}

//增加课程
void SerSecretary::addCourse(){
    print("输入想要增加的课程ID:");
    string c_id;
    std::cin>>c_id;
    if(!secbroker.courseExistYesOrNO(c_id)){
        secbroker.addtheCourse(c_id);
        return;
    }else{
        print("该课程ID已存在\n");
        return;
    }
}

//删除课程
void SerSecretary::deleteCourse(){
    print("输入想要删除的课程ID:");
    string c_id;
    std::cin>>c_id;
    if(!secbroker.courseExistYesOrNO(c_id)){
        print("该课程不存在");
        return;
    }else{
        secbroker.dedeletheCourse(c_id);
        return;
    }
}
//修改课程
void SerSecretary::changeTeachersInCourse(){
    secbroker.showCourseAndTeacher();
    secbroker.changeCT(coubroker);
}
