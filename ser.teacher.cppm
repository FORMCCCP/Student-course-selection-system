module;
#include "pqxx/pqxx"


export module ser.teacher;
import std;
import domain;
import teacherbroker;
import coursebroker;
import enrollmentbroker;
import studentbroker;

using std::string;
using std::print;

export class Serteacher{
private:
    Coursebroker Coubroker;         //课程服务层
    Enrollmentbroker enbroker;      //课程选择服务层
    Teacherbroker teabroker;        //教师服务层
    Studentbroker Stubroker;        //学生服务层

    class Teacher* teacher;

    std::vector<class Course*> teachingCourses;     //所教授的课程
public:
    Serteacher(std::shared_ptr<pqxx::connection> conn);
    bool login();
    class Teacher* getTeacher();

    void viewTeachingCourses(string id);        //查看教授的课程

    void evaluateGrade(string id);              //为学生的课程打分
};

Serteacher::Serteacher(std::shared_ptr<pqxx::connection> conn):
    Coubroker(conn), enbroker(conn), teabroker(conn),Stubroker(conn){}


bool Serteacher::login()
{
    string id, password;
    print("输入ID:");
    std::cin>>id;
    print("输入密码:");
    std::cin>>password;
    if(teabroker.handleLogin(id, password))
    {
        teacher = teabroker.returnTeacher(id);
        return true;
    }
    else
    {
        return false;
    }
}
//获取教师对象
class Teacher* Serteacher::getTeacher()
{
    return teacher;
}

//查看教授的课程
void Serteacher::viewTeachingCourses(string id)
{
    teachingCourses = teabroker.obtainTeachingCourse(id,Coubroker);
    if(teachingCourses.empty())
    {
        print("你没有要教授的课程\n");
        print("=========\n");
        return;
    }
    for(class Course* c: teachingCourses)
    {
        print("{}\n", c->getName());
        print("课程ID:{}  学分:{}  容量:{}\n",c->getId(),c->getCredit(),c->getCapacity());
        print("目前人数:{}  所授老师ID:{}\n",c->getCurrentCapacity(),c->getTeacherid());
        print("开放学期:{}  开放年级:{}\n",c->getSession(),c->getGrade());
        print("=========\n");
    }
}

void Serteacher::evaluateGrade(string id)
{
    print("输入课程ID:");
    string c_id;
    std::cin>>c_id;
    teabroker.showCourseAndStudent(c_id,id, Stubroker);
    teabroker.evaluateGradeToStudent(id);
}

