#pragma once

#include <pqxx/pqxx>
import std;

class Database {
private:
    std::shared_ptr<pqxx::connection> connection;

    // 数据库配置
    std::string host = "localhost";
    std::string port = "5432";
    std::string dbname = "scdatabase";  // 项目要求的数据库名
    std::string user = "postgres";
    std::string password = "123456";

public:
    Database();
    std::shared_ptr<pqxx::connection> getConnection();              //获取数据库连接
    void execute(const std::string& sql);                           //执行SQL语句
    void insertDatas();                                             //插入初始数据
private:
    //用于判断是否需要进行表创建和数据初始化的标记
    bool insertJudgment = false;

    std::string buildConnectionString(bool includeDbName = true);   //创建连接字符串
    bool databaseExists();                                          //检查数据库是否存在
    void createDatabase();                                          //创建数据库
    void connectToDatabase();                                       //连接数据库
    void createTables();                                            //创建选课系统所需的表
};


// 构造函数：自动初始化
Database::Database() {
    std::print("初始化数据库...\n");
    try {
        if(!databaseExists()){
            std::print("数据库不存在{}，正在创建...", dbname);
            createDatabase();
            insertJudgment = true;
        }

        connectToDatabase();

        if(insertJudgment){
            createTables();
            insertDatas();
            std::print("\n创建数据表成功\n数据表初始化成功\n");
        }
        std::print("初始化完成\n\n");

    } catch (const std::exception& e) {
        std::cerr << "数据库初始化失败: " << e.what() << std::endl;
        throw;
    }
}


// 构建连接字符串
std::string Database::buildConnectionString(bool includeDbName) {
    std::string connStr = "host=" + host + " port=" + port + " user=" + user + " password=" + password;
    if (includeDbName) {
        connStr += " dbname=" + dbname;
    }
    return connStr;
}


// 检查数据库是否存在
bool Database::databaseExists() {
        std::string connStr = buildConnectionString(false);
        connStr += " dbname=postgres";
        pqxx::connection adminConn(connStr);
        pqxx::work txn(adminConn);

        std::string sql = "SELECT 1 FROM pg_database WHERE datname = '" + dbname + "'";
        pqxx::result result = txn.exec(sql);

        txn.commit();
        return !result.empty();
}


// 创建所有需要的表
void Database::createTables() {
        // 教师表
        execute(R"(
            CREATE TABLE IF NOT EXISTS teachers (
                id VARCHAR(20) PRIMARY KEY,
                name VARCHAR(50) NOT NULL,
                password VARCHAR(50) DEFAULT '123456',
                majored VARCHAR(50)
            )
        )");
        // 学生表
        execute(R"(
            CREATE TABLE IF NOT EXISTS students (
                id  VARCHAR(20) NOT NULL,
                name VARCHAR(50) NOT NULL,
                password VARCHAR(50) DEFAULT '123456',
                major VARCHAR(50) ,
                total_credits INT DEFAULT 0,
                grade VARCHAR(20) CHECK(grade IN('大一','大二','大三','大四')),
                current_session INT CHECK(current_session IN(1,2)),
                PRIMARY KEY(id)
            )
        )");

        // 课程表
        execute(R"(
            CREATE TABLE IF NOT EXISTS courses (
                id VARCHAR(20),
                name VARCHAR(100) NOT NULL,
                credits INT NOT NULL,
                capacity INT NOT NULL,
                current_capacity INT DEFAULT 0,
                teacher_id VARCHAR(20),
                session INT CHECK(session IN(1,2)),
                grade VARCHAR(20) CHECK(grade IN('大一','大二','大三','大四')),
                majored VARCHAR(20),
                PRIMARY KEY(id),
                FOREIGN KEY(teacher_id) REFERENCES teachers(id)
            )
        )");

        // 选课记录表
        execute(R"(
            CREATE TABLE IF NOT EXISTS enrollments (
                student_id VARCHAR(20) NOT NULL REFERENCES students(id),
                course_id VARCHAR(20) NOT NULL REFERENCES courses(id),
                credit INT DEFAULT 0
            )
        )");

        // 秘书表
        execute(R"(
            CREATE TABLE IF NOT EXISTS secretarys (
                id VARCHAR(20),
                name VARCHAR(50) NOT NULL,
                password VARCHAR(50) DEFAULT '123456',
                secondaryPassword VARCHAR(50),
                PRIMARY KEY(id)
            )
        )");
}


// 获取数据库连接
std::shared_ptr<pqxx::connection> Database::getConnection() {
    if (!connection || !connection->is_open()) {
        throw std::runtime_error("数据库连接不可用");
    }
    return connection;
}


// 执行SQL语句
void Database::execute(const std::string& sql) {
    try {
        pqxx::work txn(*getConnection());
        txn.exec(sql);
        txn.commit();
    } catch (const std::exception& e) {
        std::cerr << "执行SQL失败: " << sql << std::endl;
        std::cerr << "错误: " << e.what() << std::endl;
        throw;
    }
}

//创建数据库
void Database::createDatabase(){
    try {
        std::string connStr = buildConnectionString(false);
        connStr += " dbname=postgres";

        pqxx::connection adminConn(connStr);
        // 使用非事务对象
        pqxx::nontransaction ntx(adminConn);

        std::string sql = "CREATE DATABASE \"" + dbname + "\" "
                         "ENCODING 'UTF8' "
                         "LC_COLLATE 'en_US.UTF-8' "
                         "LC_CTYPE 'en_US.UTF-8'";
        // 执行创建数据库命令
        ntx.exec(sql);

        std::print("数据库 {} 创建成功\n", dbname);

    } catch (const std::exception& e) {
        std::cerr << "创建数据库失败: " << e.what() << std::endl;
        throw;
    }
}

//与数据库连接
void Database::connectToDatabase() {
    try {
        std::string connStr = buildConnectionString(true);

        // 创建连接
        connection = std::make_shared<pqxx::connection>(connStr);

        if (connection->is_open()) {
            std::print("成功连接到数据库: {}\n",connection->dbname());
        } else {
            throw std::runtime_error("无法连接到数据库");
        }

    } catch (const pqxx::broken_connection& e) {
        std::cerr << "数据库连接失败: " << e.what() << std::endl;
        std::cerr << "连接字符串: " << buildConnectionString(true) << std::endl;
        throw;
    } catch (const std::exception& e) {
        std::cerr << "连接数据库失败: " << e.what() << std::endl;
        throw;
    }
}

//插入数据
void Database::insertDatas(){
    try{
    execute(R"(
        INSERT INTO students VALUES
        ('2024001','Saul Goodman','1233455','法律学',0,'大二',1),
        ('2023002','Mao','19660516','历史学',0,'大二',1);
    )");

    execute(R"(
        INSERT INTO teachers VALUES
        ('t0001','Mr.Q','13478','历史学'),
        ('t0320','Mr.F','26915','法学'),
        ('t0113','Mr.C','33462','历史学'),
        ('t0027','Mr.N','17189','历史学');
    )");

    execute(R"(
        INSERT INTO courses VALUES
        ('LAW002','宪法学',4,62,0,'t0320',1,'大二','法律学'),
        ('LAW003','民法学',4,58,0,'t0320',1,'大二','法律学'),
        ('HIS003','世界通史',5,64,0,'t0001',1,'大二','历史学'),
        ('HIS009','中国通史',5,64,0,'t0113',1,'大二','历史学'),
        ('HIS010','美国史',5,47,0,'t0001',1,'大二','历史学'),
        ('HIS015','基督教史',5,32,1,'t0027',1,'大二','历史学');
    )");

    execute(R"(
        INSERT INTO enrollments VALUES
        ('2023002','HIS015',0);
    )");

    execute(R"(
        INSERT INTO secretarys VALUES
        ('90078','TTT','12345','123');
        )");
    }catch(const std::exception& e){
        std::cerr<<"数据初始化失败"<< e.what() <<std::endl;
    }

}
