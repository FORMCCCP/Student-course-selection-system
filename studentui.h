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
};

StudentUI::StudentUI(std::shared_ptr<pqxx::connection> conn):
    connection(conn){}


void StudentUI::showStudentMenu(Serstudent& stu){
    print("\n===学生界面===\n");
    print("学生:{} 年级:{}\n",stu.getStudent()->getName(), stu.getStudent()->getGrade());
    print("1,查看可选课程\n2,选择课程\n3,退出课程\n4,查看我的课程\n5,查看我的成绩\n6,退出\n");
}

void StudentUI::Ser_student(){
    Serstudent stu(connection);

    if(stu.login()){
     print("登陆成功，欢迎同学{}\n",stu.getStudent()->getName());
    }else{
        print("登陆失败\n");
        return;
    }
    showStudentMenu(stu);

    while(1){
        print("请选择:");
        int choose;
        std::cin >> choose;
        switch(choose){
        case 4:
            print("\n=========\n");
            stu.viewObtaninedCourses(stu.getStudent()->getId());
            print("=========\n");
            break;
        case 6:
            std::cout << "\033[2J\033[H" << std::flush;
            return;
        default:
            print("无效选择，");
            continue;
        }
    }
}
