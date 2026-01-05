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
    string c_session;       //开放学期
    string c_grade;         //开放年级


};
