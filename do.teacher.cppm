export module domain:teacher;
import :person;
import std;
using std::string;

export class Teacher: public Person{
private:
    string collage;                                 //所属学院
    std::time_t created_at;                         //创建时间
    std::vector<class Course* > _teachingCourses;   //所授课程
public:

};
