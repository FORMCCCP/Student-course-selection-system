export module domain:teacher;
import :person;
import std;
using std::string;

export class Teacher: public Person{
private:
    string major;                                 //所属专业

public:
    Teacher(string id, string name, string password, string major);
};

Teacher::Teacher(string id, string name, string password, string m):
    major(m),Person(name, id, password){}
