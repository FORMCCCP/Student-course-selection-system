#pragma once

#include "Database.h"
#include <pqxx/pqxx>
#include <string>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <print>


class Studentbroker: public Database{
private:
    std::weak_ptr<class Student> currentStudent;
};
