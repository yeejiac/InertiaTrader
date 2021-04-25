#ifndef TRADINGDATAHANDLER_H_
#define TRADINGDATAHANDLER_H_

#include <iostream>
#include <openssl/md5.h>
#include <chrono>
#include <thread>
#include "mariaDBHandler.h"

class TradingDataHandler:public MariaDBHandler
{
public:
    TradingDataHandler(std::string iniopt);
    ~TradingDataHandler();
    std::map<std::string, std::string> getUserData();
    std::vector<std::string> getTradingData();
    bool insertOrder(OrderData *od);
    bool insertReport(std::string nid, std::string orderPrice, std::string side);
    unsigned char* md5(std::string plaintext);
    bool connstatus;
private:
};

#endif