#pragma once

#include <pqxx/pqxx>
#include <string>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <print>

class Database {
private:
    std::unique_ptr<pqxx::connection> connection;

    // 数据库配置
    std::string host = "localhost";
    std::string port = "5432";
    std::string dbname = "scdatabase";  // 项目要求的数据库名
    std::string user = "postgres";
    std::string password = "123456";

public:
    // 构造函数：自动检查并创建数据库
    Database();

    // 获取数据库连接
    pqxx::connection& getConnection();

    // 检查连接状态
    bool isConnected() const;

    // 执行SQL语句
    void execute(const std::string& sql);

    // 执行查询并返回结果
    pqxx::result query(const std::string& sql);

    // 创建选课系统所需的表
    void createTables();

private:

    std::string buildConnectionString(bool includeDbName = true);   // 创建连接字符串
    bool databaseExists();          // 检查数据库是否存在

};


// 构造函数：自动初始化
Database::Database() {
    std::print("初始化数据库...\n");
    try {
        if(!databaseExists()) throw std::runtime_error("数据库不存在");
        connection = make_unique<pqxx::connection>(buildConnectionString());

        if (connection->is_open()) {
            std::print("成功连接到数据库:{}\n" ,connection->dbname());
        } else {
            throw std::runtime_error("无法连接数据库");
        }

        createTables();
        std::print("初始化完成\n\n\n");

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
    try {
        pqxx::connection adminConn(buildConnectionString(false) + " dbname=postgres");
        pqxx::work txn(adminConn);

        std::string sql = "SELECT 1 FROM pg_database WHERE datname = '" + dbname + "'";
        pqxx::result result = txn.exec(sql);

        txn.commit();
        return !result.empty();

    } catch (...) {
        return false;
    }
}


// 创建所有需要的表
void Database::createTables() {
    try {
        // 学生表
        execute(R"(
            CREATE TABLE IF NOT EXISTS students (
                id SERIAL PRIMARY KEY,
                student_id VARCHAR(20) UNIQUE NOT NULL,
                name VARCHAR(50) NOT NULL,
                password VARCHAR(50) DEFAULT '123456',
                email VARCHAR(100),
                department VARCHAR(50),
                total_credits INT DEFAULT 0,
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            )
        )");

        // 课程表
        execute(R"(
            CREATE TABLE IF NOT EXISTS courses (
                id SERIAL PRIMARY KEY,
                course_code VARCHAR(20) UNIQUE NOT NULL,
                course_name VARCHAR(100) NOT NULL,
                credits INT NOT NULL,
                capacity INT NOT NULL,
                current_enrollment INT DEFAULT 0,
                is_open BOOLEAN DEFAULT true,
                teacher_id INT,
                teacher_name VARCHAR(50),
                time_slot VARCHAR(50),
                classroom VARCHAR(50),
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            )
        )");

        // 选课记录表
        execute(R"(
            CREATE TABLE IF NOT EXISTS enrollments (
                id SERIAL PRIMARY KEY,
                student_id INT NOT NULL REFERENCES students(id),
                course_id INT NOT NULL REFERENCES courses(id),
                status VARCHAR(20) DEFAULT 'enrolled',
                grade FLOAT DEFAULT -1,
                enrollment_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                UNIQUE(student_id, course_id)
            )
        )");

        // 教师表
        execute(R"(
            CREATE TABLE IF NOT EXISTS teachers (
                id SERIAL PRIMARY KEY,
                employee_id VARCHAR(20) UNIQUE NOT NULL,
                name VARCHAR(50) NOT NULL,
                department VARCHAR(50),
                email VARCHAR(100)
            )
        )");


    } catch (const std::exception& e) {
        std::cerr << "创建表失败: " << e.what() << std::endl;
        throw;
    }
}


// 获取数据库连接
pqxx::connection& Database::getConnection() {
    if (!connection || !connection->is_open()) {
        throw std::runtime_error("数据库连接不可用");
    }
    return *connection;
}

// 检查连接状态
bool Database::isConnected() const {
    return connection && connection->is_open();
}

// 执行SQL语句
void Database::execute(const std::string& sql) {
    try {
        pqxx::work txn(getConnection());
        txn.exec(sql);
        txn.commit();
    } catch (const std::exception& e) {
        std::cerr << "执行SQL失败: " << sql << std::endl;
        std::cerr << "错误: " << e.what() << std::endl;
        throw;
    }
}

// 执行查询
pqxx::result Database::query(const std::string& sql) {
    try {
        pqxx::work txn(getConnection());
        pqxx::result result = txn.exec(sql);
        txn.commit();
        return result;
    } catch (const std::exception& e) {
        std::cerr << "查询失败: " << sql << std::endl;
        std::cerr << "错误: " << e.what() << std::endl;
        throw;
    }
}
