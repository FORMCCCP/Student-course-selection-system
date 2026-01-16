#pragma once

#include "broker.h"

class Teacherbroker: public Broker{
public:
    Teacherbroker(std::shared_ptr<pqxx::connection> conn);

};
Teacherbroker::Teacherbroker(std::shared_ptr<pqxx::connection> conn):
    Broker(conn){}
