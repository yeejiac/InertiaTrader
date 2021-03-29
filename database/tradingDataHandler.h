#ifndef TRADINGDATAHANDLER_H_
#define TRADINGDATAHANDLER_H_

#include <iostream>
#include "mariaDBHandler.h"

class TradingDataHandler:public MariaDBHandler
{
public:
    TradingDataHandler(std::string iniopt);
    ~TradingDataHandler();
private:
};

#endif