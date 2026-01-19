//Student-course-selection-system
//学生课程管理系统
//writer：15小组：龙玟吉，高歌，王耀
//time: 19-01-2019
//用户可以通过三种身份登陆到该系统
//学生：选课、退课、查看课
//教师：查看、打分
//秘书：管理学生、教师和课程、分配教师教学课程

import Database;
import userui;

int main()
{
    Database db;
    UserUI ui(db.getConnection());
    ui.WelcomeMenu();
    return 0;
}
