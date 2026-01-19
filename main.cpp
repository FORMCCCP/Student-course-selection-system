import Database;
import userui;

int main()
{
    Database db;
    UserUI ui(db.getConnection());
    ui.WelcomeMenu();
    return 0;
}

