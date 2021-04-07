#include "trader.h"

RiskController::RiskController(){}

// void RiskController::verify(Order *order, double priceNow)
// {
//     //委託單基本風控
//     if(order->getPrice()>priceNow*1.1||order->getPrice()<priceNow*0.9) //委託價檢核
//     {
//         order->setStatus(OrderStatus::FAILED);
//     }
//     else
//     {
//         order->setStatus(OrderStatus::FAILED);
//     }
//     Report *rpt = new Report(order);
// }

Order::Order(std::string text, int stockNum):stockNum_(stockNum)
{
    nid_ = std::stol(text.substr(0, text.find(DELIMITER)));
    price_ = std::stoi(text.substr(1, text.find(DELIMITER)));
    side_ = static_cast<Side>(std::stoi(text.substr(2, text.find(DELIMITER))));
    market_ = static_cast<Market>(std::stoi(text.substr(3, text.find(DELIMITER))));
    ordertype_ = (OrderType)std::stoi(text.substr(4, text.find(DELIMITER)));
    timeString_ = text.substr(5, text.find(DELIMITER));
}

void Order::setStatus(OrderStatus status)
{
    orderstatus_ = status;
}

OrderStatus Order::getStatus()
{
    return orderstatus_;
}

void Order::setside(Side side)
{
    side_ = side;
}

Side Order::getside()
{
    return side_;
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

// std::string Report::composeReport()
// {
//     std::string reportTex = "";
//     if(reportType_ == ReportType::NONE)
//     {
//         std::cout<<"lack of report type"<<std::endl;
//     }
//     else if(reportType_ == ReportType::OrderReport)
//     {
//         std::cout<<"OrderReport"<<std::endl;
//         reportTex = "100|" + std::to_string(nid_) + '|' + orderNum_ + '|' + std::to_string(price_) + '|' + static_cast<Side>(side_) + 
//                     '|' + static_cast<Market>(market_) + '|' + static_cast<OrderType>(ordertype_) + '|' + timeString_;
//     }
//     else if(reportType_ == ReportType::ExecutionReport)
//     {
//         std::cout<<"ExecutionReport"<<std::endl;
//         reportTex = "102|" + std::to_string(nid_) + '|' + orderNum_ + '|' + std::to_string(price_) + '|' + static_cast<Side>(side_) + 
//                     '|' + static_cast<Market>(market_) + '|' + static_cast<OrderType>(ordertype_) + '|' + timeString_;
//     }
//     else
//     {
//         std::cout<<"ErrorReport"<<std::endl;
//         reportTex = "101|" + std::to_string(nid_) + '|' + orderNum_ + '|' + std::to_string(price_) + '|' + static_cast<Side>(side_) + 
//                     '|' + static_cast<Market>(market_) + '|' + static_cast<OrderType>(ordertype_) + '|' + timeString_;
//     }
//     return reportTex;
// }

Trader::Trader()
{
    //logwrite->write(LogLevel::DEBUG, "Virtual trader initialise");
}

void Trader::setTraderStatus(bool status)
{
    traderstatus_ = status;
}

bool Trader::getTraderStatus()
{
    return traderstatus_;
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

// void Trader::matchup()
// {
//     while(getTraderStatus())
//     {
//         for(auto b = buyside_.begin(); b != buyside_.end();) 
//         {
//             for(auto s = sellside_.begin(); s != sellside_.end();) 
//             {
//                 if(*b->getPrice() == *s->getPrice())
//                 {
//                     Report *report_buy = new Report(&b);
//                     Report *report_sell = new Report(&s);
//                     b = buyside_.erase(b);
//                     s = sellside_.erase(s);
//                 }
//                 else
//                     ++s;
//             }
//             ++b;
//         }
//         std::this_thread::sleep_for(std::chrono::seconds(10)); //十秒撮合一次
//     }
// }

void Trader::getOrder()
{
    while(getTraderStatus())
    {
        if(sr->dq->checkSpace() > 0)
        {
            orderDataInsert(sr->dq->popDTA());
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
        // std::unique_lock<std::mutex> lk(cv_m);
        // cv_.wait_for(lk, 1, []{return Trader::dqstatus;});
        
    }
}

// void Trader::sendReport()
// {
//     while(getTraderStatus())
//     {
//         std::unique_lock<std::mutex> lk(cv_m);
//         cv_.wait_for(lk, 1, [&]{return reportList_.size() > 0;});
//         for(int i = 0; i<reportList_.size();i++)
//         {
//             std::string str = reportList_[i]->composeReport();
//             //需要修改socket class來識別每一個連線的ID，才能根據每個ID來取得Connection 物件
//             sr->getConnectionObject(0)->sendto(str);
//         }
//     }
// }

void Trader::startTransaction()
{
    //整體交易流程
    // 1. 啟動socket開啟競價模式
    // 2. 收委託單並進行風控
    // 3. 發送回報資料(委託回報或成交回報)
    // 4. 媒合委託單
    sr = new Server("./doc/settings.ini", "socket", "./log/");
    std::thread orderReceive(&Trader::getOrder, this);
    // std::thread matchup(&Trader::matchup, this);

    orderReceive.detach();
    // matchup.detach();
}