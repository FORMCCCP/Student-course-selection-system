export module domain:student;
import :person;
import std;
using std::string;

export class Student: public Person{
private:
    string major;                //专业
    int s_credits;               //学分
    int current_session;      //当前学期(1表示上学期，2表示下学期)
    string grade;                //年级(大一, 大二，大三，大四)
public:
    Student(string name, string id, string password, string m, int c, int cs,
    string g);

    string getName();
    string getId();
    string getGrade();
};

Student::Student(string name, string id, string password, string m, int c, int cs,
    string g):
    major(m),s_credits(c), current_session(cs),
    grade(g),Person(name, id, password){}

string Student::getName(){
    return std::format ("{}", p_name);
}
string Student::getGrade(){
    return std::format ("{}", grade);
}

string Student::getId(){
    return std::format ("{}", p_id);
}
