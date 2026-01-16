#pragma once

#include <pqxx/pqxx>
import std;
import domain;

class Broker{
private:
    std::shared_ptr<pqxx::connection> conn;
public:
    Broker(std::shared_ptr<pqxx::connection> c);

    template<class... Args>
    void execute(const std::string& sql, Args&&... args);           //执行SQL语句
    template<class... Args>
    pqxx::result query(const std::string& sql, Args&&... args);     //执行查询并返回结果
    bool isConnected();                                             //检查连接状态
};
Broker::Broker(std::shared_ptr<pqxx::connection> c):
    conn(c){}


// 执行SQL语句
template<class... Args>
void Broker::execute(const std::string& sql, Args&&... args) {
    try {
        pqxx::work txn(*conn);
        if constexpr (sizeof...(args) == 0) {
            txn.exec(sql);
        } else {
            txn.exec_params(sql, std::forward<Args>(args)...);
        }
        txn.commit();
    } catch (const std::exception& e) {
        std::cerr << "执行SQL失败: " << sql << std::endl;
        std::cerr << "错误: " << e.what() << std::endl;
        throw;
    }
}


// 检查连接状态
bool Broker::isConnected(){
    return conn && conn->is_open();
}

// 执行查询
template<class... Args>
pqxx::result Broker::query(const std::string& sql, Args&&... args) {
    try {
        pqxx::work txn(*conn);
        pqxx::result result;
        if constexpr (sizeof...(args) == 0) {
            result = txn.exec(sql);
        } else {
            result = txn.exec_params(sql, std::forward<Args>(args)...);
        }
        txn.commit();
        return result;
    } catch (const std::exception& e) {
        std::cerr << "查询失败: " << sql << std::endl;
        std::cerr << "错误: " << e.what() << std::endl;
        throw;
    }
}
