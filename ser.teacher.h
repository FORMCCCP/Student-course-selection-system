#pragma once

import domain;
#include <pqxx/pqxx>
#include "teacherbroker.h"
#include "coursebroker.h"
#include "enrollmentbroker.h"


class Serteacher{
private:
    Coursebroker Coubroker;         //课程服务层
    Enrollmentbroker enbroker;      //课程选择服务层
    Teacherbroker teabroker;        //教师服务层

    class Teacher* teacher;

    std::vector<class Course*> teachingCourses;     //所教授的课程
public:
    Serteacher(std::shared_ptr<pqxx::connection> conn);
    void login();
    class Teacher* getTeacher();

    void viewTeachingCourses(string id);        //查看教授的课程

    void evaluateGrade(string id);              //为学生的课程打分
};

Serteacher::Serteacher(std::shared_ptr<pqxx::connection> conn):
    Coubroker(conn), enbroker(conn), teabroker(conn){}

bool Serteacher::login()
{
    string id, password;
    print("输入ID:");
    std::cin>>id;
    print("输入密码:");
    std::cin>>password;
    if(Teabroker.handleLogin(id, password))
    {
        teacher = Teabroker.returnStudent(id);
        return true;
    }
    else
    {
        return false;
    }
}
//获取教师对象
class Teacher* Serteacher::getTeacher()
{
    return teacher;
}

//查看教授的课程
void Serteacher::viewTeachingCourses(string id)
{
    teachingCourses(id);
    if(teachingCourses.empty())
    {
        print("你没有要教授的课程\n");
        print("=========\n");
        return;
    }
    for(class Course* c: teachingCourses)
    {
        print("{}\n", c->getName());
        print("课程ID:{}  学分:{}  容量:{}\n",c->getId(),c->getCredit(),c->getCapacity());
        print("目前人数:{}  所授老师ID:{}\n",c->getCurrentCapacity(),c->getTeacherid());
        print("开放学期:{}  开放年级:{}\n",c->getSession(),c->getGrade());
        print("=========\n");
    }
}

void Serteacher::evaluateGrade(string id)
{
    teabroker.evaluateGradeToStudent(c_id,id,Stubroker);
}
