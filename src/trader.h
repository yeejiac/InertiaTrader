#ifndef TRADER_H_
#define TRADER_H_

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <map>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>

#include "enumStorage.h"
#include "server.h"
#include "../funclib/logwriter.h"
#include "../database/tradingDataHandler.h"

#define DELIMITER '|'

class Order
{
public:
    Order();
    ~Order();
    std::string nid; //網單編號
    int volumn; //委託量
    double orderPrice; //委託價
    std::string symbol; //商品代號
    std::string userID; //委託人帳號
    long getNid();
    void setNid();
    void setside(Side side);
    Side getside();
    void setPrice();
    double getPrice();
    void setStatus(OrderStatus status);
    OrderStatus getStatus();
    int connId;
private:
    int stockNum_;
    long nid_;
    Side side_;
    OrderType ordertype_;
    Market market_;
    double price_;
    std::string timeString_;
    OrderStatus orderstatus_;
};

class Report
{
public:
    Report();
    void setReportType(ReportType rpt);
    std::string composeReport(Order *order);
    long generateNid();
private:
    int stockNum_;
    std::string orderNum_;
    long nid_;
    Side side_;
    OrderType ordertype_;
    Market market_;
    double price_;
    std::string timeString_;
    OrderStatus orderstatus_;
    ReportType reportType_;
    int nid_sub1 = rand() % INT32_MAX;
    int nid_sub2 = 1;
};

class Stock
{
public:
    Stock();
    ~Stock();
private:
    int stockNum_;
    double priceNow_;
    double priceMax_;
    double priceMin_;
};

class RiskController
{
public:
    RiskController();
    ~RiskController();
    void verify(Order *order, double priceNow);
private:
    std::string originalText;
};

class Trader
{
public:
    Trader(bool mode);
    ~Trader();
    void essentialData_initialise();
    void setTraderStatus(bool status);
    bool getTraderStatus();
    void rawStrHandle(std::string rawStr);
    void orderDataInsert(Order *order);
    void matchup();
    void loadInitialise();
    void getOrder();
    void sendExecReport(Order *order);
    int checkDataQueue();
    void startTransaction();
    Server *sr;
    Order *od;
    OrderData *odt;
    Report *rpt;
    RiskController *rc;
    Logwriter *logwrite;
    TradingDataHandler *db;
    bool dqstatus = false;
    bool serverstatus = false;
    bool testmode;
private:
    std::vector<Order*> buyside_;
    std::vector<Order*> sellside_;
    std::vector<Report*> reportList_;
    std::vector<std::string> productList_;
    std::vector<std::string> tradeBasicData_;
    bool traderstatus_;
    std::condition_variable cv_;
    std::mutex cv_m;
    bool login_flag_ = false;
};

#endif