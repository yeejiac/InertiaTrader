#ifndef TRADER_H_
#define TRADER_H_

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <map>
#include <mutex>
#include <condition_variable>
#include <thread>

#include "enumStorage.h"
#include "server.h"

#define DELIMITER '|'

class RiskController
{
public:
    RiskController();
    ~RiskController();
    void verify();
private:
    std::string originalText;
};

class Trader
{
public:
    Trader();
    void rawStrHandle(std::string rawStr);
    void orderDataInsert(std::string str);
    void matchup();
    ~Trader();
    Logwriter *logwrite;
private:
    std::vector<Order*> buyside_;
    std::vector<Order*> sellside_;
    std::vector<Stock*> stockList_;
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