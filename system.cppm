export module system;

import domain;
import ui;

export class System{
private:
    System();

public:
    static System& singleton();
    void run();
};

System& System::singleton(){
    static System instance;
    return instance;
}
System::System(){}

void System::run(){

}
