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

#define DELIMITER '|'

Logwriter *logwrite;

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
    Trader();
    ~Trader();
    void setTraderStatus(bool status);
    bool getTraderStatus();
    void rawStrHandle(std::string rawStr);
    void orderDataInsert(std::string str);
    void matchup();
    void loadInitialise();
    void getOrder();
    void sendReport();
    int checkDataQueue();
    void startTransaction();
    Logwriter *logwrite;
    Server *sr;
private:
    std::vector<Order*> buyside_;
    std::vector<Order*> sellside_;
    std::vector<Report*> reportList_;
    std::map<std::string, Stock*> stockList_;
    bool traderstatus_;
    std::condition_variable cv_;
    std::mutex cv_m;
};

class Order
{
public:
    Order(std::string text, int stockNum);
    ~Order();
    long getNid();
    void setNid();
    void setside();
    Side getside();
    void setPrice();
    double getPrice();

    void setStatus(OrderStatus status);
    OrderStatus getStatus();
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
    Report(Order *order);
    void setReportType(ReportType rpt);
    std::string composeReport();
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

#endif