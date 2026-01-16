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
