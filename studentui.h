#pragma once

#include <pqxx/pqxx>
#include "ser.student.h"

using std::string;
using std::print;

class StudentUI{
private:
    std::shared_ptr<pqxx::connection> connection;

public:
    StudentUI(std::shared_ptr<pqxx::connection> conn);
    void showStudentMenu(Serstudent& stu);  //显示学生界面
    void Ser_student();                     //处理输入

    void clearScreen();                     //屏幕清洁
    void waitForEnter();
};



StudentUI::StudentUI(std::shared_ptr<pqxx::connection> conn):
    connection(conn){}


void StudentUI::showStudentMenu(Serstudent& stu){
    print("===学生界面===\n");
    print("学生:{} 年级:{} 专业:{}\n",
          stu.getStudent()->getName(),
          stu.getStudent()->getGrade(),
          stu.getStudent()->getMajor());
    print("1,查看可选课程\n2,选择课程\n3,退出课程\n4,查看我的课程\n5,查看我的成绩\n6,退出\n");
}

void StudentUI::Ser_student(){
    Serstudent stu(connection);

    if(stu.login()){
     print("登陆成功! 欢迎同学{}\n\n",stu.getStudent()->getName());
    }else{
        print("登陆失败");
        waitForEnter();
        clearScreen();
        return;
    }
    showStudentMenu(stu);

    while(1){
        print("请选择功能:");
        int choose;
        std::cin >> choose;
        switch(choose){
        case 1:
            print("\n以下是你可以选择的课程:\n");
            print("=========\n");
            stu.viewAvailableCourses(stu.getStudent()->getId());
            print("\n");
            waitForEnter();
            clearScreen();
            showStudentMenu(stu);
            break;
        case 2:
            print("\n请输入你想选择的课程ID:");
            stu.chooseCourse(stu.getStudent()->getId());
            waitForEnter();
            clearScreen();
            showStudentMenu(stu);
            break;
        case 3:
            print("\n输入你想退出的课程ID:");
            stu.exitCourse(stu.getStudent()->getId());
            waitForEnter();
            clearScreen();
            showStudentMenu(stu);
            break;
        case 4:
            print("\n以下是你的课程:\n");
            print("=========\n");
            stu.viewObtaninedCourses(stu.getStudent()->getId());
            print("\n");
            waitForEnter();
            clearScreen();
            showStudentMenu(stu);
            break;
        case 5:
            print("\n=========\n");
            stu.viewTotalGrade(stu.getStudent()->getId());
            print("=========\n");
            waitForEnter();
            clearScreen();
            showStudentMenu(stu);
            break;
        case 6:
            clearScreen();
            return;
        default:
            print("无效选择，");
            continue;
        }
    }
}


void StudentUI::clearScreen() {
    std::cout << "\033[2J\033[H" << std::flush;
}

void StudentUI::waitForEnter() {
    std::cout << "\n按回车键返回菜单...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
    clearScreen();
}
