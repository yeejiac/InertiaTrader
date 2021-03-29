#ifndef MARIADBHAMDLER_H_
#define MARIADBHAMDLER_H_

#include <iostream>
#include <mysql/mysql.h>
#include <memory>
#include <string.h>

#include "../funclib/initParser.h"
#include "../funclib/logwriter.h"
#include "table.h"

class MariaDBHandler
{
public:
    MariaDBHandler();
    ~MariaDBHandler();
    bool initialise(std::string cfg); 
    void showTableItems(std::string sqlcommand);
    void insertOrderData(struct OrderData);
    Logwriter *logwrite = new Logwriter("DB", "../log/");
    const char* MY_HOSTNAME;
    const char* MY_DATABASE;
    const char* MY_USERNAME;
    const char* MY_PASSWORD;
    const char* MY_SOCKET;
    unsigned int MY_PORT;
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
private:
};

#endif