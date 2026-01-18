export module domain:teacher;
import :person;
import std;
using std::string;

export class Teacher: public Person{
private:
    string major;                                 //所属专业

public:
    Teacher(string id, string name, string password, string major);
    string getName();       //获取名字
    string getId();         //获取ID
    string getMajor         //获取专业
};

Teacher::Teacher(string id, string name, string password, string m):
    major(m),Person(name, id, password){}

string Teacher::getName()
{
    return std::format ("{}", p_name);
}

string Teacher::getId()
{
    return std::format ("{}", p_id);
}

string Teacher::getMajor()
{
    return std::format ("{}", major);
}
