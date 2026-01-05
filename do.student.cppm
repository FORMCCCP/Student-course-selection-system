export module domain:student;
import :person;
import std;
using std::string;

export class Student: public Person{
private:
    string major;                //专业
    int s_credits;               //学分
    std::time_t created_at;      //创建时间
    string current_session;      //当前学期
    string grade;                //年级
    std::vector<class Course*> _courses; //已择选程
};
