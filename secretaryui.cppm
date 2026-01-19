module;
#include "pqxx/pqxx"

export module secretaryui;
import std;
import ser.secretary;

using std::string;
using std::print;

export class SecretaryUI{
private:
    std::shared_ptr<pqxx::connection> connection;
public:
    SecretaryUI(std::shared_ptr<pqxx::connection> conn);
    void showsecretaryMenu(SerSecretary& stu);  //显示秘书界面
    void Ser_secretary();                     //处理输入

    void clearScreen();                       //屏幕清洁
    void waitForEnter();

};

SecretaryUI::SecretaryUI(std::shared_ptr<pqxx::connection> conn):
    connection(conn){}

//显示秘书界面
void SecretaryUI::showsecretaryMenu(SerSecretary& sec){
    print("===秘书界面===\n");
    print("秘书:{} \n",
          sec.getSercretary()->getName());
    print("1,增加学生\n2,删除学生\n3,增加教师\n4,删除教师\n5,增加课程\n6,删除课程\n7,修改课程教师\n8,退出\n");
}

void SecretaryUI::Ser_secretary(){
    SerSecretary sec(connection);

    if(sec.login()){
        print("登陆成功，欢迎秘书{}\n\n",sec.getSercretary()->getName());
    }else{
        print("登陆失败");
        waitForEnter();
        clearScreen();
        return;
    }
    showsecretaryMenu(sec);
    while(1){
        print("请选择功能:");
        int choose;
        std::cin>>choose;
        switch(choose){
        case 1:
            std::println();
            if(sec.verificationSecondaryPassword()){
                print("验证成功\n\n");
                sec.addStudent();
            }else{
                print("验证失败\n\n");
            }
            waitForEnter();
            clearScreen();
            showsecretaryMenu(sec);
            break;
        case 2:
            std::println();
            if(sec.verificationSecondaryPassword()){
                print("验证成功\n\n");
                sec.deleteStudent();
            }else{
                print("验证失败\n\n");
            }
            waitForEnter();
            clearScreen();
            showsecretaryMenu(sec);
            break;
        case 3:
            std::println();
            if(sec.verificationSecondaryPassword()){
                print("验证成功\n\n");
                sec.addTeacher();
            }else{
                print("验证失败\n\n");
            }
            waitForEnter();
            clearScreen();
            showsecretaryMenu(sec);
            break;
        case 4:
            std::println();
            if(sec.verificationSecondaryPassword()){
                print("验证成功\n\n");
                sec.deleteTeacher();
            }else{
                print("验证失败\n\n");
            }
            waitForEnter();
            clearScreen();
            showsecretaryMenu(sec);
            break;
        case 5:
            std::println();
            if(sec.verificationSecondaryPassword()){
                print("验证成功\n\n");
                sec.addCourse();
            }else{
                print("验证失败\n\n");
            }
            waitForEnter();
            clearScreen();
            showsecretaryMenu(sec);
            break;
        case 6:
            std::println();
            if(sec.verificationSecondaryPassword()){
                print("验证成功\n\n");
                sec.deleteCourse();
            }else{
                print("验证失败\n\n");
            }
            waitForEnter();
            clearScreen();
            showsecretaryMenu(sec);
            break;
        case 7:
            std::println();
            if(sec.verificationSecondaryPassword()){
                print("验证成功\n\n");
                sec.changeTeachersInCourse();
            }else{
                print("验证失败\n\n");
            }
            waitForEnter();
            clearScreen();
            showsecretaryMenu(sec);
            break;
        case 8:
            clearScreen();
            return;
        default:
            print("无效选择，");
            continue;
        }
    }
}



void SecretaryUI::clearScreen() {
    std::cout << "\033[2J\033[H" << std::flush;
}

void SecretaryUI::waitForEnter() {
    std::cout << "\n按回车键返回菜单...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
    clearScreen();
}
