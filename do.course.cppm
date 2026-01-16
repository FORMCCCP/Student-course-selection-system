export module domain:course;
import std;

using std::string;

export class Course{
private:
    string c_id;            //课程ID
    string c_name;          //课程名
    short c_credit;         //该课程学分
    int c_capacity;         //人数容量
    int current_capacity;   //当前人数
    string c_teacherId;     //授课教师ID
    int c_session;          //开放学期
    string c_grade;         //开放年级
    string majored;         //所属专业
public:
    Course(string id,string name,short credit,int cap,int cu_cap,string teaID,int ses,
    string g, string m);
    void showCourse();

    string getId();
    string getName();
    short getCredit();
    int getCapacity();
    int getCurrentCapacity();
    string getTeacherid();
    string getSession();
    string getGrade();

};

Course::Course(string id,string name,short credit,int cap,int cu_cap,string teaID,int ses,
    string g, string m):
    c_id(id),c_name(name),c_credit(credit),c_capacity(cap),current_capacity(cu_cap),
    c_teacherId(teaID),c_session(ses),c_grade(g),majored(m){}

void Course::showCourse(){
    print("{}\n", c_name);
    print("课程ID:{}  学分:{}  容量:{}\n",c_id, c_credit, c_capacity);
    print("目前人数:{}  所授老师ID:{}\n",current_capacity, c_teacherId);
    print("开放学期:{}  开放年级:{}\n",c_session, c_grade);
}

string Course::getId(){
    return std::format("{}", c_id);
}
string Course::getName(){
    return std::format("{}", c_name);
}
short Course::getCredit(){
    short c = c_credit;
    return c;
}
int Course::getCapacity(){
    int c = c_capacity;
    return c;
}
int Course::getCurrentCapacity(){
    int c = current_capacity;
    return c;
}
string Course::getTeacherid(){
    return std::format("{}", c_teacherId);
}
string Course::getSession(){
    return std::format("{}", c_session);
}
string Course::getGrade(){
    return std::format("{}",c_grade);
}
