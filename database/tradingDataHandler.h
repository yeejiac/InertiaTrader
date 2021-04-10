#ifndef TRADINGDATAHANDLER_H_
#define TRADINGDATAHANDLER_H_

#include <iostream>
#include <openssl/md5.h>
#include "mariaDBHandler.h"

class TradingDataHandler:public MariaDBHandler
{
public:
    TradingDataHandler(std::string iniopt);
    ~TradingDataHandler();
    std::map<std::string, std::string> getUserData();
    bool insertOrder();
    unsigned char* md5(std::string plaintext);
private:
};

#endif