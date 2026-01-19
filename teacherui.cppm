//表现层：教师UI
//处理教师登陆和功能选择

module;
#include "pqxx/pqxx"

export module teacherui;

import std;
import ser.teacher;

using std::string;
using std::print;
export class TeacherUI
{
private:
    std::shared_ptr<pqxx::connection> connection;
public:
    TeacherUI(std::shared_ptr<pqxx::connection> conn);
    void showTeacherMenu(Serteacher& tea);  //显示教师界面
    void Ser_teacher();                     //处理输入

    void clearScreen();                     //屏幕清洁
    void waitForEnter();
};

TeacherUI::TeacherUI(std::shared_ptr<pqxx::connection> conn):
    connection(conn){}

void TeacherUI::showTeacherMenu(Serteacher& tea)
{
    print("===教师界面===\n");
    print("教师:{} 专业:{}\n",
        tea.getTeacher()->getName(),
        tea.getTeacher()->getMajor());
    print("1,查看已教授课程\n2,评定成绩\n3,退出\n");
}

void TeacherUI::Ser_teacher()
{
    Serteacher tea(connection);

    if(tea.login()){
     print("登陆成功! 欢迎老师{}\n\n",tea.getTeacher()->getName());
    }else{
        print("登陆失败");
        waitForEnter();
        clearScreen();
        return;
    }
    showTeacherMenu(tea);

    while(1)
    {
        print("请选择功能:");
        int choose;
        std::cin >> choose;
        if (std::cin.fail()) {
            print("无效输入，请输入数字\n");
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;}
        switch(choose)
        {
        case 1:
            print("\n以下是已教授的课程:\n");
            print("========\n");
            tea.viewTeachingCourses(tea.getTeacher()->getId());
            print("\n");
            waitForEnter();
            clearScreen();
            showTeacherMenu(tea);
            break;
        case 2:
            tea.evaluateGrade(tea.getTeacher()->getId());
            waitForEnter();
            clearScreen();
            showTeacherMenu(tea);
            break;
        case 3:
            clearScreen();
            return;
        default:
            print("无效选择，");
            continue;
        }
    }
}

void TeacherUI::clearScreen() {
    std::cout << "\033[2J\033[H" << std::flush;
}

void TeacherUI::waitForEnter() {
    std::cout << "\n按回车键返回菜单...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
    clearScreen();

}
