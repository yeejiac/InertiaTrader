#include "tradingDataHandler.h"

TradingDataHandler::TradingDataHandler(std::string iniopt)
{
    if(initialise(iniopt))
        logwrite->write(LogLevel::DEBUG, "(MariaDB) DB connect success ");
    else
        logwrite->write(LogLevel::ERROR, "(MariaDB) DB connect failed ");
}