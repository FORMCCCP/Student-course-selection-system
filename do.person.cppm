export module domain:person;
import std;

using std::string;

export class Person{
protected:
    string p_name;          //姓名
    string p_id;            //ID
    string p_password;      //密码
public:
    Person(string name, string id, string pass);
    virtual ~Person();

};

Person::Person(string name, string id, string pass):
    p_name(name), p_id(id), p_password(pass){}
Person::~Person(){}

