//服务层/逻辑应用层：学生处理
//处理学生相关的功能业务

module;
#include "pqxx/pqxx"

export module ser.student;
import std;
import domain;
import studentbroker;
import coursebroker;
import enrollmentbroker;

using std::string;
using std::print;

export class Serstudent{
private:
    Studentbroker Stubroker;        //学生服务层
    Coursebroker Coubroker;         //课程服务层
    Enrollmentbroker enbroker;      //课程选择服务层


    class Student* student;

    std::vector<class Course*> obtainedCourses;   //已经获取课程
    bool needRefreshObtained = true;              //刷新缓存记号
    std::vector<class Course*> unobtainedCourses; //未获取但可选择课程
    bool needRefreshunobtained = true;            //刷新缓存记号

public:
    Serstudent(std::shared_ptr<pqxx::connection> conn);
    bool login();                           //处理登陆
    class Student* getStudent();            //获取学生对象

    void ObtainedCourses(string id);        //获得已选择的课程
    void viewObtaninedCourses(string id);   //查看我的课程

    void AvailableCourses(string id);       //获得可以选择的课程
    void viewAvailableCourses(string id);   //查看可以选择的课程

    void chooseCourse(string id);           //选择课程
    void exitCourse(string id);             //退出课程

    void viewTotalGrade(string id);         //计算成绩
};

Serstudent::Serstudent(std::shared_ptr<pqxx::connection> conn):
     Stubroker(conn), Coubroker(conn), enbroker(conn){}

//处理登陆
bool Serstudent::login(){
    string id, password;
    print("输入ID:");
    std::cin>>id;
    print("输入密码:");
    std::cin>>password;
    if(Stubroker.handleLogin(id, password)){
        student = Stubroker.returnStudent(id);
        return true;
    }else{
        return false;
    }
}
//获取学生对象
class Student* Serstudent::getStudent(){
    return student;
}

//获得已经选择的课程
void Serstudent::ObtainedCourses(string s_id){
    if (!needRefreshObtained && obtainedCourses.size() != 0) {
        return;
    }
    for (Course* course : obtainedCourses) {
        delete course;
    }
    obtainedCourses.clear();
    std::vector<class Course*> courses = enbroker.returnObtainCourses(s_id, Coubroker);
    obtainedCourses = courses;
    needRefreshObtained = false;
    return;
}
//查看我的课程
void Serstudent::viewObtaninedCourses(string s_id){
    ObtainedCourses(s_id);
    if(obtainedCourses.empty()){
        print("你没有选择课程\n");
        print("=========\n");
        return;
    }
    for(class Course* c: obtainedCourses){
        print("{}\n", c->getName());
        print("课程ID:{}  学分:{}  容量:{}\n",c->getId(),c->getCredit(),c->getCapacity());
        print("目前人数:{}  所授老师ID:{}\n",c->getCurrentCapacity(),c->getTeacherid());
        print("开放学期:{}  开放年级:{}\n",c->getSession(),c->getGrade());
        print("=========\n");
    }
}
//获得可以选择的课程
void Serstudent::AvailableCourses(string s_id){
    if(!needRefreshunobtained && unobtainedCourses.size() != 0)return;
    for (Course* course : unobtainedCourses) {
        delete course;
    }
    std::vector<class Course*> courses = Stubroker.checkAvailableCourse(s_id, Coubroker);
    unobtainedCourses = courses;
    needRefreshunobtained = false;
    return;
}

//查看可以选择的课程
void Serstudent::viewAvailableCourses(string id){
    AvailableCourses(id);
    if(unobtainedCourses.empty()){
        print("你没有可选课程\n");
        return;
    }
    for(class Course*c: unobtainedCourses){
        print("{}\n", c->getName());
        print("课程ID:{}  学分:{}  容量:{}\n",c->getId(),c->getCredit(),c->getCapacity());
        print("目前人数:{}  所授老师ID:{}\n",c->getCurrentCapacity(),c->getTeacherid());
        print("开放学期:{}  开放年级:{}\n",c->getSession(),c->getGrade());
        print("=========\n");
    }
}

//选择课程
void Serstudent::chooseCourse(string s_id){
    string c_id;
    std::cin>>c_id;
    Stubroker.chooseCourseOperation(c_id, s_id,Coubroker);
    needRefreshObtained = true;
    needRefreshunobtained = true;
}

//退出课程
void Serstudent::exitCourse(string s_id){
    string c_id;
    std::cin>>c_id;
    Stubroker.exitCourseOperatrion(c_id,s_id,Coubroker);
    needRefreshObtained = true;
    needRefreshunobtained = true;
}

//计算成绩
void Serstudent::viewTotalGrade(string s_id){
    int grade = Stubroker.calculateTotalGrade(s_id, Coubroker);
    if(grade == -1){
        print("你未选择相应课程\n");
        return;
    }else{
        print("\n你的总成绩: {}\n",grade);
        return;
    }
}
