#include "mariaDBHandler.h"

MariaDBHandler::MariaDBHandler(){}

MariaDBHandler::~MariaDBHandler()
{
    mysql_close(conn);
}

bool MariaDBHandler::initialise(std::string cfg)
{
    std::unique_ptr<InitParser> ip(new InitParser("../doc/settings.ini", "database"));
    ip->readLine();
    MY_HOSTNAME = ip->iniContainer["addr"].c_str();
    MY_PORT = 3306;
    MY_USERNAME = ip->iniContainer["user"].c_str();
    MY_PASSWORD = ip->iniContainer["password"].c_str();
    MY_DATABASE = ip->iniContainer["database"].c_str();
    MY_SOCKET = NULL;
    try 
    {
        conn = mysql_init(NULL);
        // Establish a MySQL connection
        conn = mysql_real_connect(conn, MY_HOSTNAME, MY_USERNAME, MY_PASSWORD, MY_DATABASE, MY_PORT, MY_SOCKET, 0);
        if(!conn)
            logwrite->write(LogLevel::ERROR, "(MariaDB) Connection Failed ");  
    } 
    catch (char *e) 
    {
        std::string str(e);
        logwrite->write(LogLevel::ERROR, "(MariaDB) [EXCEPTION] " + str);
        return false;
    }
    return true;
}

void MariaDBHandler::showTableItems(std::string sqlcommand)
{
    if(!conn)
        return;
    
    mysql_query(conn, sqlcommand.c_str());
    res = mysql_store_result(conn);

    int num_fields = mysql_num_fields(res);
    // Fetch all rows from the result
    while ((row = mysql_fetch_row(res)))
    {
       // Print all columns
       for(int i = 0; i < num_fields; i++)
       {
           // Make sure row[i] is valid!
           if(row[i] != NULL)
                std::cout << row[i] << std::endl;
           else
                std::cout << "NULL" << std::endl;
       }
    }
}

int main()
{
    MariaDBHandler *dbm = new MariaDBHandler();
    if(dbm->initialise("database"))
        std::cout<<"success"<<std::endl;;
    dbm->showTableItems("select * from test1");
}

// g++ -std=c++11 ../lib/libcommon.so -lmysqlclient mariaDBHandler.cpp -o test.out