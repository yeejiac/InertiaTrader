#include "trader.h"

Order::Order(std::string text, int stockNum):stockNum_(stockNum)
{
    nid_ = std::stol(text.substr(0, text.find(DELIMITER)));
    price_ = std::stoi(text.substr(1, text.find(DELIMITER)));
    side_ = std::stoi(text.substr(2, text.find(DELIMITER)));
    market_ = std::stoi(text.substr(3, text.find(DELIMITER)));
    ordertype_ = std::stoi(text.substr(4, text.find(DELIMITER)));
    timeString_ = text.substr(5, text.find(DELIMITER));
}

void Report::setReportType(ReportType rpt)
{
    reportType_ = rpt;
}

long Report::generateNid()
{
    ulong unsignedKey = (((ulong) nid_sub1) << 32) | nid_sub2;
    long key = (long) unsignedKey;
    nid_sub2++;
    return key;
}

std::string Report::composeReport()
{
    std::string reportTex = "";
    if(reportType_ == ReportType::NONE)
    {
        std::cout<<"lack of report type"<<std::endl;
    }
    else if(reportType_ == ReportType::OrderReport)
    {
        std::cout<<"OrderReport"<<std::endl;
        reportTex = "100|" + std::to_string(nid_) + '|' + orderNum_ + '|' + price_ + '|' + side_ + '|' + market_ + '|' + ordertype_ + '|' + timeString_;
    }
    else if(reportType_ == ReportType::ExecutionReport)
    {
        std::cout<<"ExecutionReport"<<std::endl;
        reportTex = "102|" + std::to_string(nid_) + '|' + orderNum_ + '|' + price_ + '|' + side_ + '|' + market_ + '|' + ordertype_ + '|' + timeString_;
    }
    else
    {
        std::cout<<"123454321"<<std::endl;
        std::cout<<" 1234321 "<<std::endl;
        std::cout<<"  12321  "<<std::endl;
        std::cout<<"   121   "<<std::endl;
        std::cout<<"    1    "<<std::endl;
        std::cout<<"ErrorReport"<<std::endl;
        reportTex = "101|" + std::to_string(nid_) + '|' + orderNum_ + '|' + price_ + '|' + side_ + '|' + market_ + '|' + ordertype_ + '|' + timeString_;
    }
    return reportTex;
}

Trader::Trader()
{
    logwrite->write(LogLevel::DEBUG, "Virtual trader initialise");
}

void Trader::rawStrHandle(std::string rawStr)
{
    char strdelimiter = '&';
    size_t pos = 0;
    std::string token;
    while ((pos = rawStr.find(strdelimiter)) != std::string::npos) 
    {
        token = rawStr.substr(0, pos);
        std::cout << token << std::endl;
        orderDataInsert(token);
        rawStr.erase(0, pos + 1);
    }
    orderDataInsert(rawStr);
}

void Trader::orderDataInsert(std::string str)
{
    //New Order object and insert data
    Order *order = new Order(str, 9527);
    if(order->getside() == Side::BUY)
        buyside_.push_back(order);
    else
        sellside_.push_back(order);
}

void Trader::matchup()
{
    std::vector<Order>::iterator ptr; 
    std::vector<Order>::iterator ptr2;
    for(ptr = buyside_.begin(); ptr < buyside_.end(); ptr++)
    {
        for(ptr2 = sellside_.begin(); ptr2 < sellside_.end(); ptr2++)
        {
            // if(*ptr->)
        }
    }
}