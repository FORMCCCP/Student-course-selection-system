export module domain:person;
import std;

using std::string;

export class Person{
private:
    string p_name;          //姓名
    string p_id;            //ID
    string p_password;      //密码
public:
    Person(string name, string id, string pass);
    virtual ~Person();
    string info();
    bool hasId(string id);
};

Person::Person(string name, string id, string pass):
    p_name(name), p_id(id), p_password(pass){}
Person::~Person(){}

string Person::info(){
    return std::format("{}", p_name);
}

bool Person::hasId(string id){
    return id == p_id;
}
