#include "tradingDataHandler.h"

TradingDataHandler::TradingDataHandler(std::string iniopt)
{
    int i = 0;
    while(!initialise(iniopt))
    {
        i++;
        logwrite->write(LogLevel::ERROR, "(MariaDB) DB connect failed ");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if(i>5)
            return;
        connstatus = false;
    }
    logwrite->write(LogLevel::DEBUG, "(MariaDB) DB connect success "); 
    connstatus = true; 
}

TradingDataHandler::~TradingDataHandler()
{
    mysql_free_result(res);
    mysql_close(conn);
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
    logwrite->write(LogLevel::DEBUG, "(MariaDB) sql command : " + sqlcommand);

    mysql_query(conn, sqlcommand.c_str());
    res = mysql_store_result(conn);
    while ((row = mysql_fetch_row(res))) 
    {
        result.insert(std::pair<std::string, std::string>(row[0], row[1]));
    }

    // std::map<std::string, std::string>::iterator it;
    // for(it = result.begin(); it != result.end(); it++)
    // {
    //     std::cout<<it->first<<" "<<it->second<<std::endl;
    // }
    mysql_free_result(res);
    return result;
}

std::vector<std::string> TradingDataHandler::getTradingData()
{
    std::vector<std::string> result;
    if(!conn)
    {
        logwrite->write(LogLevel::ERROR, "(MariaDB) DB connect failed ");
        return result;
    }
    std::string sqlcommand = "SELECT * FROM stock.InitData ORDER BY ID DESC LIMIT 1;";
    logwrite->write(LogLevel::DEBUG, "(MariaDB) sql command : " + sqlcommand);
    mysql_query(conn, sqlcommand.c_str());
    res = mysql_store_result(conn);
    int num_fields = mysql_num_fields(res);
    while ((row = mysql_fetch_row(res))) 
    {
        for(int i = 0 ; i<num_fields;i++)
        {
            result.push_back(row[i]);
        }
    }
    mysql_free_result(res);
    return result;
}

std::vector<std::string> TradingDataHandler::getProductList()
{
    std::vector<std::string> result;
    if(!conn)
    {
        logwrite->write(LogLevel::ERROR, "(MariaDB) DB connect failed ");
        return result;
    }
    std::string sqlcommand = "SELECT `product_id` FROM stock.ProductList ;";
    logwrite->write(LogLevel::DEBUG, "(MariaDB) sql command : " + sqlcommand);
    mysql_query(conn, sqlcommand.c_str());
    res = mysql_store_result(conn);
    int num_fields = mysql_num_fields(res);
    while ((row = mysql_fetch_row(res))) 
    {
        for(int i = 0 ; i<num_fields;i++)
        {
            result.push_back(row[i]);
        }
    }
    mysql_free_result(res);
    return result;
}

bool TradingDataHandler::insertOrder(OrderData *od)
{
    std::string value = od->nid + ","  + std::to_string(od->orderPrice) + "," +"'"+ od->symbol +"'" +"," + od->userID + "," + std::to_string(od->side);
    std::string query = "INSERT INTO `stock`.`Order` (`NID`, `OrderPrice`, `Symbol`, `UserID`, `Side`) VALUES (" + value + ");";
    std::cout<<query<<std::endl;
    if (mysql_query(conn, query.c_str()) != 0)                   
    {    
        // fprintf(stderr, "%s\n", mysql_error(conn));     
        std::string msg(mysql_error(conn));                                                                                                                                                   
        logwrite->write(LogLevel::ERROR, "(MariaDB) [EXCEPTION] Query Failure " + msg);       
        return false;                                                                  
    }
    else
    {
        logwrite->write(LogLevel::DEBUG, "(MariaDB) Insert Order Success");
        return true;
    }
        
}

bool TradingDataHandler::insertReport(std::string nid, std::string orderPrice, std::string side)
{
    std::string value = nid + ","  + orderPrice +  "," + side;
    std::string query = "INSERT INTO `stock`.`ExecReport` (`NID`, `ExecPrice`, `Side`) VALUES (" + value + ");";
    std::cout<<query<<std::endl;
    if (mysql_query(conn, query.c_str()) != 0)                   
    {    
        // fprintf(stderr, "%s\n", mysql_error(conn));     
        std::string msg(mysql_error(conn));                                                                                                                                                   
        logwrite->write(LogLevel::ERROR, "(MariaDB) [EXCEPTION] Query Failure " + msg);        
        return false;                                                                  
    }
    else
    {
        logwrite->write(LogLevel::DEBUG, "(MariaDB) Insert Report Success");
        return true;
    }
        
}

// int main()
// {
//     TradingDataHandler *db = new TradingDataHandler("database");
//     std::cout<<db->getUserData("0324027")<<std::endl;
// }