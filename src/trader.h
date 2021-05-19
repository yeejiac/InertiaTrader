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
#include <set>

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
    void setSituation(OrderSituation situation);
    OrderSituation getSituation();
    int connId;
    int side;
private:
    int stockNum_;
    long nid_;
    Side side_;
    OrderType ordertype_;
    Market market_;
    double price_;
    std::string timeString_;
    OrderStatus orderstatus_; //經過風控後委託狀態
    OrderSituation ordersituation_; //委託單狀態
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
    void verify(Order *order);
    std::vector<std::string> productList;
    std::vector<std::string> tradeBasicData;
private:
    std::string originalText;
    
};

class Trader
{
public:
    Trader(bool mode);
    ~Trader();
    void essentialData_initialise();
    void loadExistOrder(); //Disconnect mechanism
    void setTraderStatus(bool status);
    bool getTraderStatus();
    void rawStrHandle(std::string rawStr);
    void orderDataInsert(Order *order);
    void matchup();
    void loadInitialise();
    void getOrder();
    void getCancelOrder();
    void sendExecReport(Order *order);
    void generateExecReport();
    int checkDataQueue();
    void startTransaction();
    void endTransaction();
    Server *sr;
    Order *od;
    OrderData *odt;
    Report *rpt;
    RiskController *rc = new RiskController();
    Logwriter *logwrite;
    TradingDataHandler *db;
    bool dqstatus = false;
    bool serverstatus = false;
    bool testmode;
    Side sideFlag;
private:
    std::vector<Order*> buyside_;
    std::vector<Order*> sellside_;
    std::vector<std::shared_ptr<Order*>> reportList_;
    // std::multiset<Order*> rawBuyside_;
    // std::multiset<Order*> rawSellside_;
    bool traderstatus_;
    std::condition_variable cv_;
    std::mutex cv_m;
    bool login_flag_ = false;
};

#endif