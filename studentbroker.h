#pragma once

#include "broker.h"
import domain;

using std::string;

class Studentbroker: public Broker{
private:
    std::shared_ptr<pqxx::connection> connection;

public:
    Studentbroker(std::shared_ptr<pqxx::connection> conn);  //构造函数
    bool handleLogin(string id, string password);           //验证账号密码
    class Student* returnStudent(string id);
};

Studentbroker::Studentbroker(std::shared_ptr<pqxx::connection> conn):
    Broker(conn),connection(conn){}

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
        std::cout << "\033[2J\033[H" << std::flush;
        std::print("不存在该ID\n");
        return false;
    }

    if(!result_pw.empty()){
        std::cout << "\033[2J\033[H" << std::flush;
        return true;
    }else{
        std::cout << "\033[2J\033[H" << std::flush;
        std::print("密码错误\n");
        return false;
        }
}

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
