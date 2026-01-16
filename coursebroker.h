#pragma once

#include "broker.h"
import domain;


class Coursebroker: public Broker{
private:
    std::shared_ptr<pqxx::connection> connection;
public:
    Coursebroker(std::shared_ptr<pqxx::connection> conn);
    class Course* returnCourse(string c_id);    //返回课程对象

};

Coursebroker::Coursebroker(std::shared_ptr<pqxx::connection> conn):
    Broker(conn),connection(conn){}

class Course* Coursebroker::returnCourse(string c_id){
    pqxx::result result = query(
                R"(SELECT * FROM courses
                    where id=$1)",c_id
                                );
    string id = result[0][0].as<string>();            //课程ID
    string name = result[0][1].as<string>();          //课程名
    short credit = result[0][2].as<short>();         //该课程学分
    int capacity = result[0][3].as<int>();         //人数容量
    int current_capacity = result[0][4].as<int>();   //当前人数
    string teacherId =result[0][5].as<string>();     //授课教师ID
    int session =result[0][6].as<int>();       //开放学期
    string grade =result[0][7].as<string>();         //开放年级
    string majored=result[0][8].as<string>();       //所属专业

    Course* cou_ptr = new Course(id,name,credit,capacity,current_capacity,teacherId,session,grade,majored);
    return cou_ptr;
}

