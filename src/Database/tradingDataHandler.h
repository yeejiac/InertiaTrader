#ifndef TRADINGDATAHANDLER_H_
#define TRADINGDATAHANDLER_H_

#include <iostream>
#include <openssl/md5.h>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "mariaDBHandler.h"
#include "../../funclib/dataQueue.h"

class TradingDataHandler:public MariaDBHandler
{
public:
    TradingDataHandler(std::string iniopt);
    ~TradingDataHandler();
    // std::map<std::string, std::string> getUserData();
    std::map<std::string, UserData*> getUserData();
    std::vector<std::string> getTradingData();
    std::vector<std::string> getProductList();
    std::vector<OrderData*> getExistOrder();
    bool addTrader(UserData *ud);
    bool insertOrder(OrderData *od);
    bool insertReport(std::string nid, std::string orderPrice, std::string execPrice, std::string side, std::string client_serialNum);
    bool updateOrderSituation(std::string nid, std::string status);
    bool updateOrderPrice(std::string nid, std::string price);
    void sqlCommandExec();
    // unsigned char* md5(std::string plaintext);
    bool connstatus;
    DataQueue *sql_dq = new DataQueue(100);
private:
    std::mutex mutex_;
	std::condition_variable cv_;
};

#endif