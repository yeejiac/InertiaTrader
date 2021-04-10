#include "tradingDataHandler.h"

TradingDataHandler::TradingDataHandler(std::string iniopt)
{
    if(initialise(iniopt))
        logwrite->write(LogLevel::DEBUG, "(MariaDB) DB connect success ");
    else
        logwrite->write(LogLevel::ERROR, "(MariaDB) DB connect failed ");
}

unsigned char* TradingDataHandler::md5(std::string plaintext)
{
    unsigned char result[MD5_DIGEST_LENGTH];
    MD5((unsigned char*)plaintext.c_str(), plaintext.size(), result);
    return result;
}

std::map<std::string, std::string> TradingDataHandler::getUserData()
{
    std::map<std::string, std::string> result;
    if(!conn)
    {
        logwrite->write(LogLevel::ERROR, "(MariaDB) DB connect failed ");
        return result;
    }
    std::string sqlcommand = "select * from stock.User ;";
    logwrite->write(LogLevel::ERROR, "(MariaDB) sql command : " + sqlcommand);

    mysql_query(conn, sqlcommand.c_str());
    res = mysql_store_result(conn);
    while ((row = mysql_fetch_row(res))) 
    {
        result.insert(std::pair<std::string, std::string>(row[0], row[1]));
    }

    std::map<std::string, std::string>::iterator it;
    for(it = result.begin(); it != result.end(); it++)
    {
        std::cout<<it->first<<" "<<it->second<<std::endl;
    }
    mysql_free_result(res);
    return result;
}

// int main()
// {
//     TradingDataHandler *db = new TradingDataHandler("database");
//     std::cout<<db->getUserData("0324027")<<std::endl;
    
// }