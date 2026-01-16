#pragma once

import std;
#include <pqxx/pqxx>
#include "ser.teacher.h"

using std::string;

class TeacherUI{
private:
    std::shared_ptr<pqxx::connection> connection;
public:
    TeacherUI(std::shared_ptr<pqxx::connection> conn);
    void showTeacherMenu(Serteahcer& tea);  //显示教师界面
    void Ser_teacher();                     //处理输入

    void clearScreen();                     //屏幕清洁
    void waitForEnter();
};

TeacherUI::TeacherUI(std::shared_ptr<pqxx::connection> conn):
    connection(conn){}


void Ser_teacher(){
    Serteacher(connection);




}
