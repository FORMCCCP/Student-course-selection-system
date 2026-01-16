#pragma once

#include "broker.h"
#include "coursebroker.h"
import domain;

using std::string;

class Enrollmentbroker : public Broker{
private:
    std::shared_ptr<pqxx::connection> connection;

public:
    Enrollmentbroker(std::shared_ptr<pqxx::connection> conn);
    std::vector<class Course*> returnObtainCourses(string std_id, Coursebroker& Coubroker);     //返回已经获取的课程
};

Enrollmentbroker::Enrollmentbroker(std::shared_ptr<pqxx::connection> conn):
    Broker(conn),connection(conn){}



std::vector<class Course*> Enrollmentbroker::returnObtainCourses(string std_id, Coursebroker& Coubroker){
    pqxx::result result = query(
                R"(SELECT course_id FROM enrollments
                    where student_id=$1)",std_id
                                );
    if(result.empty()){
        return std::vector<Course*>{};}

    std::vector<string> courses;
    size_t count = result.size();
    for(int i=0; i<count; ++i){
        string str = result[i][0].as<string>();
        courses.push_back(str);
    }

    std::vector<class Course*> obtainedCourses;
    for(string& s : courses){
        Course* cou_ptr = Coubroker.returnCourse(s);
        obtainedCourses.push_back(cou_ptr);
    }
    return obtainedCourses;
}
