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

std::string TradingDataHandler::getUserData(std::string username)
{
    if(!conn)
    {
        logwrite->write(LogLevel::ERROR, "(MariaDB) DB connect failed ");
        return "";
    }
    std::string sqlcommand = "select `password` from stock.User Where `user` = " + username + ";";
    std::cout<<sqlcommand<<std::endl;
    
    try
    {
        mysql_query(conn, sqlcommand.c_str());
        res = mysql_store_result(conn);
        std::string hashval(reinterpret_cast<char*>(md5(mysql_fetch_row(res)[0])));
        return hashval;
    }
    catch(...)
    {
        logwrite->write(LogLevel::DEBUG, "(MariaDB) sql failed or no data ");
        return "error";
    }
}

int main()
{
    TradingDataHandler *db = new TradingDataHandler("database");
    std::cout<<db->getUserData("0324027")<<std::endl;
    
}