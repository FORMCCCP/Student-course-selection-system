export module domain:secretary;
import :person;
import std;

using std::string;

export class Secretary: public Person{
private:
    string secondaryPassword;
public:
    Secretary(string id, string name,string password,string secpassword);
    string getName();
    string getSecondary();
};

Secretary::Secretary(string id, string name,string password,string secpassword):
    Person(name,id,password), secondaryPassword(secpassword){}

string Secretary::getName(){
    return std::format("{}",p_name);
}

string Secretary::getSecondary(){
    return std::format("{}",secondaryPassword);
}
