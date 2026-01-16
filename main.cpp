#include "Database.h"
#include "userui.h"

int main()
{
    Database db;
   // db.insertDatas();
    UserUI ui(db.getConnection());
    ui.WelcomeMenu();
    return 0;
}

