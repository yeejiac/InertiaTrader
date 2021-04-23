#include "trader.h"

RiskController::RiskController(){}

void RiskController::verify(Order *order, double priceNow = 60)
{
    //委託單基本風控
    order->setStatus(OrderStatus::VERIFIED);
    // if(order->orderPrice>priceNow*1.1||order->orderPrice<priceNow*0.9) //委託價檢核
    // {
    //     order->setStatus(OrderStatus::VERIFIED);
    // }
    // else
    // {
    //     order->setStatus(OrderStatus::FAILED);
    // }
    // Report *rpt = new Report(order);
}

Order::Order()
{
    // nid_ = std::stol(text.substr(0, text.find(DELIMITER)));
    // price_ = std::stoi(text.substr(1, text.find(DELIMITER)));
    // side_ = static_cast<Side>(std::stoi(text.substr(2, text.find(DELIMITER))));
    // market_ = static_cast<Market>(std::stoi(text.substr(3, text.find(DELIMITER))));
    // ordertype_ = (OrderType)std::stoi(text.substr(4, text.find(DELIMITER)));
    // timeString_ = text.substr(5, text.find(DELIMITER));
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

double Order::getPrice()
{
    return price_;
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

std::string Report::composeReport(Order *order)
{
    std::string reportTex = "";
    if(reportType_ == ReportType::NONE)
    {
        std::cout<<"lack of report type"<<std::endl;
    }
    else if(reportType_ == ReportType::OrderReport)
    {
        std::cout<<"OrderReport"<<std::endl;
        reportTex = "100|success" ;
        // reportTex = "100|" + std::to_string(nid_) + '|' + orderNum_ + '|' + std::to_string(price_) + '|';
    }
    // else if(reportType_ == ReportType::ExecutionReport)
    // {
    //     std::cout<<"ExecutionReport"<<std::endl;
    //     reportTex = "102|" + std::to_string(nid_) + '|' + orderNum_ + '|' + std::to_string(price_) + '|' + static_cast<Side>(side_) + 
    //                 '|' + static_cast<Market>(market_) + '|' + static_cast<OrderType>(ordertype_) + '|' + timeString_;
    // }
    else
    {
        std::cout<<"ErrorReport"<<std::endl;
        reportTex = "100|failed" ;
        // reportTex = "101|" + std::to_string(nid_) + '|' + orderNum_ + '|' + std::to_string(price_) + '|';
    }
    return reportTex;
}

Trader::Trader(bool mode):testmode(mode)
{
    logwrite = new Logwriter("SR", "./log/");
    logwrite->write(LogLevel::DEBUG, "Virtual trader initialise");
    setTraderStatus(true);
}

void Trader::setTraderStatus(bool status)
{
    traderstatus_ = status;
}

bool Trader::getTraderStatus()
{
    return traderstatus_;
}

// void Trader::rawStrHandle(std::string rawStr)
// {
//     char strdelimiter = '&';
//     size_t pos = 0;
//     std::string token;
//     while ((pos = rawStr.find(strdelimiter)) != std::string::npos) 
//     {
//         token = rawStr.substr(0, pos);
//         std::cout << token << std::endl;
//         orderDataInsert(token);
//         rawStr.erase(0, pos + 1);
//     }
//     orderDataInsert(rawStr);
// }

void Trader::orderDataInsert(Order *order)
{
    //New Order object and insert data
    if(order->getside() == Side::BUY)
    {
        buyside_.push_back(order);
        logwrite->write(LogLevel::DEBUG, "(Trader) Get Buyside Order");
    }
    else
    {
        sellside_.push_back(order);
        logwrite->write(LogLevel::DEBUG, "(Trader) Get SellSide Order");
    }
    cv_m.unlock();    
}

void Trader::matchup()
{
    logwrite->write(LogLevel::DEBUG, "(Trader) Match up thread start");
    std::unique_lock<std::mutex> lk1(cv_m);
    cv_.wait(lk1, [this]{return sr->getconnStatus();});
    while(getTraderStatus())
    {
        cv_m.lock();
        logwrite->write(LogLevel::DEBUG, "(Trader) Do Match up process");
        for(int i = 0; i <buyside_.size();i++) 
        {
            for(int j = 0; j <sellside_.size();j++) 
            {
                if(buyside_[i]->getPrice() == sellside_[j]->getPrice())
                {
                    logwrite->write(LogLevel::DEBUG, "(Trader) Match up success");
                    sendExecReport(buyside_[i]);
                    sendExecReport(sellside_[j]);
                    buyside_.erase(buyside_.begin() + i);
                    sellside_.erase(sellside_.begin() + j);
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(10)); //十秒撮合一次
    }
}

void Trader::getOrder()
{
    logwrite->write(LogLevel::DEBUG, "(Trader) Get Order thread start");
    // std::unique_lock<std::mutex> lk1(cv_m);
    // cv_.wait(lk1, [this]{return sr->getconnStatus();});
    while(getTraderStatus())
    {
        // std::unique_lock<std::mutex> lk2(cv_m);
        // cv_.wait(lk2, [this]{return sr->dq->checkSpace() != 0;});
        if(sr->dq->checkSpace()>0)
        {
            logwrite->write(LogLevel::DEBUG, "(Trader) Handle Order Msg");
            std::vector<std::string> res = split(sr->dq->popDTA(), "|");
            od = new Order;
            od->nid = res[1];
            od->orderPrice = std::stod(res[2]);
            od->setside(Side(std::stoi(res[3])));
            od->symbol = "TXO";
            od->userID = "0324027";
            od->connId = std::stoi(res[6]);
            rc->verify(od);
            if(od->getStatus() == OrderStatus::VERIFIED)
            {
                sr->sendToClient(std::stoi(res[6]), res[1] + "|success");
                orderDataInsert(od);
                logwrite->write(LogLevel::DEBUG, "(Trader) Input data to db");
                odt = new OrderData;
                odt->nid = od->nid;
                odt->orderPrice = od->orderPrice;
                odt->symbol = od->symbol;
                odt->userID = od->userID;
                odt->side = static_cast<int>(od->getside());
                sr->insertOrderToDB(odt);
            }
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
        
    }
}

void Trader::sendExecReport(Order *order)
{
    sr->sendToClient(order->connId, order->nid + "|OrderExec");
    sr->insertReportToDB(order->nid, std::to_string(order->orderPrice), std::to_string(static_cast<int>(od->getside())));
}

void Trader::startTransaction()
{
    //整體交易流程
    // 1. 啟動socket開啟競價模式
    // 2. 收委託單並進行風控
    // 3. 發送回報資料(委託回報或成交回報)
    // 4. 媒合委託單
    
    sr = new Server("./doc/settings.ini", "socket", "./log/", testmode);
    if(sr->getconnStatus()&&testmode == false)
    {
        std::thread orderReceive(&Trader::getOrder, this);
        std::thread matchup(&Trader::matchup, this);
        matchup.detach();
        orderReceive.detach();
    }
    else
    {
        serverstatus = true;
    }

    std::unique_lock<std::mutex> lk1(cv_m);
    cv_.wait(lk1, [this]{return serverstatus;});
    
    
    
    
}