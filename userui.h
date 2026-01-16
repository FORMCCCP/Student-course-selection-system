#pragma once

import std;
#include <pqxx/pqxx>
#include "studentui.h"
#include "secretaryui.h"
#include "teacherui.h"

using std::print;

class UserUI {
private:
    std::shared_ptr<pqxx::connection> connection;

public:
    UserUI(std::shared_ptr<pqxx::connection> conn);
    ~UserUI();
    void WelcomeMenu();         // 欢迎界面功能选择
    void loginMenu();           // 登陆界面
};

UserUI::UserUI(std::shared_ptr<pqxx::connection> conn):
    connection(conn){}

UserUI::~UserUI(){
    print("\n系统关闭，感谢你的使用\n\n");
}


void UserUI::WelcomeMenu(){

    print("=====欢迎进入课程管理系统=====\n");
    print("      1,进入登陆界面\n      2,退出系统\n请选择:");

    while(1){


        int choose;
        if(!(std::cin >> choose)){
            std::cin.clear();
            std::cin.ignore();
            print("输入错误，请重新输入正确的数字:");
            continue;
        }
        switch(choose){
            case 1:{
                loginMenu();
                break;
                }
            case 2:
                return;
            default:
                print("\n无效输入，请重输:");
        }

        print("=====欢迎进入课程管理系统=====\n");
        print("      1,进入登陆界面\n      2,退出系统\n请选择:");
    }
}

void UserUI::loginMenu(){
    std::cout << "\033[2J\033[H" << std::flush;

    print("====选择登陆方式====\n");
    print("   1,学生登陆\n   2,教师登陆\n   3,秘书登陆\n请选择:");
    while(1){
        int choose;
        std::cin >> choose;
        switch(choose){
            case 1:{
                std::println();
                std::unique_ptr<class StudentUI> StuUi = std::make_unique<class StudentUI>(connection);
                StuUi->Ser_student();
            }
            break;
            case 2:{
                std::println();
                std::unique_ptr<class TeacherUI> TeaUi = std::make_unique<class TeacherUI>(connection);
                TeaUi->Ser_teacher();
            }
            break;
            case 3:{

            }
            default:
                print("无效输入，请重输:");
                continue;
            }
        return;
    }
}


