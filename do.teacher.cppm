export module domain:teacher;
import :person;
import std;
using std::string;

export class Teacher: public Person{
private:
    string major;                                 //所属专业

    std::vector<class Course* > _teachingCourses;   //所授课程
public:

};
