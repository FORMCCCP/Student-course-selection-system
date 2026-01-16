#pragma once

import domain;
#include <pqxx/pqxx>
#include "studentbroker.h"
#include "coursebroker.h"
#include "enrollmentbroker.h"

using std::string;
using std::print;

class Serstudent{
private:
    std::shared_ptr<pqxx::connection> connection;
    Studentbroker Stubroker;
    Coursebroker Coubroker;
    Enrollmentbroker enbroker;

    string id;
    string password;
    class Student* student;

    std::vector<class Course*> obtainedCourses;   //已经获取课程
    std::vector<class Course*> unobtainedCourses; //未获取可选择课程

public:
    Serstudent(std::shared_ptr<pqxx::connection> conn);
    bool login();                           //处理登陆
    class Student* getStudent();            //获取学生对象
    void ObtainedCourses(string id);        //获得已选择的课程
    void viewObtaninedCourses(string id);   //查看我的课程
};

Serstudent::Serstudent(std::shared_ptr<pqxx::connection> conn):
    connection(conn), Stubroker(conn), Coubroker(conn), enbroker(conn){}

//处理登陆
bool Serstudent::login(){
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


void Serstudent::ObtainedCourses(string id){
    if(obtainedCourses.size() != 0){
        return;
    }
    std::vector<class Course*> courses = enbroker.returnObtainCourses(id, Coubroker);
    obtainedCourses = courses;
    return;
}

void Serstudent::viewObtaninedCourses(string id){
    ObtainedCourses(id);
    if(obtainedCourses.empty()){
        print("你没有选择课程\n");
        return;
    }
    for(class Course* c: obtainedCourses){
        print("{}\n", c->getName());
        print("课程ID:{}  学分:{}  容量:{}\n",c->getId(),c->getCredit(),c->getCapacity());
        print("目前人数:{}  所授老师ID:{}\n",c->getCurrentCapacity(),c->getTeacherid());
        print("开放学期:{}  开放年级:{}\n",c->getSession(),c->getGrade());
    }
}

