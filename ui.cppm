export module ui;
import std;
using std::print;

export class MainUI{
public:
    void userUI();
    void studentUI();
    void teacherUI();
};

void MainUI::userUI(){
    print("===欢迎进入课程管理系统===\n");
    print("1,进入登陆界面\n2,进入注册界面\n3,退出系统\n请输入:");
    while(1){
        int choose;
        std::cin>>choose;
        switch(choose){
            case 1:
                studentUI();
                break;
            case 2:
                print("2");
                break;
            case 3:
                print("3");
                break;
            default:
                print("无效选择，请重新输入：");
                break;
                }
            }
        }

void MainUI::studentUI(){}
