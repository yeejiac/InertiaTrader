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
    std::string getUserData(std::string username);
    unsigned char* md5(std::string plaintext);
private:
};

#endif