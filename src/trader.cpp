#include "trader.h"

RiskController::RiskController(){}

void RiskController::verify(Order *order)
{
    //委託單基本風控
    std::vector<std::string>::iterator it = std::find(productList.begin(), productList.end(), order->symbol);
    if(order->orderPrice>std::stod(tradeBasicData[2])*1.1||order->orderPrice<std::stod(tradeBasicData[2])*0.9||it==productList.end()) //委託價檢核
    {
        order->setStatus(OrderStatus::FAILED);
    }
    else
    {
        
        order->setStatus(OrderStatus::VERIFIED);
    }
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

void Order::setSituation(OrderSituation situation)
{
    ordersituation_ = situation;
}

OrderSituation Order::getSituation()
{
    return ordersituation_;
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
    logwrite = new Logwriter("TD", "./log/");
    logwrite->write(LogLevel::DEBUG, "Virtual trader initialise");
    essentialData_initialise();
    loadExistOrder();
    setTraderStatus(true);
}

long Trader::generateNid()
{
    ulong unsignedKey = (((ulong) nid_sub1) << 32) | nid_sub2;
    long key = (long) unsignedKey;
    nid_sub2++;
    return key;
}

void Trader::setTraderStatus(bool status)
{
    traderstatus_ = status;
}

bool Trader::getTraderStatus()
{
    return traderstatus_;
}

void Trader::essentialData_initialise()
{
    db = new TradingDataHandler(testmode?"Testdatabase":"database");
	if(db->connstatus)
    {
        rc->tradeBasicData = db->getTradingData();
        rc->productList = db->getProductList();
    }
	else
		logwrite->write(LogLevel::ERROR, "(Trader) Initial failed");
}

void Trader::loadExistOrder()
{
    logwrite->write(LogLevel::DEBUG, "(Trader) start load exist order");
    std::vector<OrderData*> existOrder = db->getExistOrder();
    logwrite->write(LogLevel::DEBUG, "(Trader) exist order volume : " + existOrder.size());
    for(int i = 0; i<existOrder.size(); i++)
    {
        Order *od = new Order();
        od->nid = existOrder[i]->nid;
        od->orderPrice = existOrder[i]->orderPrice;
        od->side = existOrder[i]->side;
        od->client_serialNum = existOrder[i]->client_serialnum;
        if(existOrder[i]->side == 1)
            buyside_.push_back(std::move(od));
        else
            sellside_.push_back(std::move(od));
    }
    logwrite->write(LogLevel::DEBUG, "(Trader) load exist order success");
}

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
        std::lock_guard<std::mutex> lck(cv_m);
        logwrite->write(LogLevel::DEBUG, "(Trader) Do Match up process");
        int num = sellside_.size();
        int num2 = buyside_.size();
        int i = 0;
        while(i <num&&num2>0&&num>0) 
        {
            logwrite->write(LogLevel::DEBUG, "(Trader) Execute Match up");
            for(int j = 0; j<num2;j++)
            {
                if(buyside_[j]->orderPrice >= sellside_[i]->orderPrice)
                {
                    buyside_[j]->execPrice = sellside_[i]->orderPrice;
                    sellside_[i]->execPrice = sellside_[i]->orderPrice;
                    sr->quoteUpdate("KKC", sellside_[i]->execPrice);
                    std::shared_ptr<Order*> a = std::make_shared<Order*> (std::move(buyside_[j]));
                    reportList_.push_back(a);
                    std::shared_ptr<Order*> b = std::make_shared<Order*> (std::move(sellside_[i]));
                    reportList_.push_back(b);
                    sellside_.erase(sellside_.begin() + i);
                    buyside_.erase(buyside_.begin() + j);
                    logwrite->write(LogLevel::DEBUG, "(Trader) Match up success");
                    j = num2;
                }
            }
            break;
        }
        logwrite->write(LogLevel::DEBUG, "(Trader) Match up process done");
        if(&num2>0||num>0)
            std::this_thread::sleep_for(std::chrono::seconds(5));
        else
            std::this_thread::sleep_for(std::chrono::seconds(1));
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
        // cv_.wait_for(lk2, std::chrono::seconds(1),[this]{return sr->dq->checkSpace() > 0;});
        if(sr->dq->checkSpace()>0)
        {
            logwrite->write(LogLevel::DEBUG, "(Trader) Handle Order Msg");
            std::vector<std::string> res = split(sr->dq->popDTA(), "|");
            od = new Order;
            od->nid = res[1];
            od->orderPrice = std::stod(res[2]);
            od->setside(res[3]=="1"?Side::BUY: Side::SELL);
            od->side = std::stoi(res[3]);
            od->symbol = res[6];
            od->userID = "0324027";
            std::cout<<res[7]<<std::endl;
            od->connId = std::stoi(res[7]);
            od->setSituation(OrderSituation::NORMAL);
            rc->verify(od);
            if(od->getStatus() == OrderStatus::VERIFIED)
            {
                Connection *cn = sr->getConnObject(od->connId);
                std::map<std::string, UserData*>::iterator it = sr->userList.find(cn->username);
                if(it->second->balance>od->orderPrice*1000)
                {
                    od->client_serialNum = generateNid();
                    sideFlag = Side(std::stoi(res[3]));
                    orderDataInsert(od);
                    logwrite->write(LogLevel::DEBUG, "(Trader) Input data to db");
                    odt = new OrderData;
                    odt->nid = od->nid;
                    odt->orderPrice = od->orderPrice;
                    odt->symbol = od->symbol;
                    odt->userID = od->userID;
                    odt->side = static_cast<int>(od->getside());
                    odt->client_serialnum = od->client_serialNum;
                    std::cout<<res[7]<<std::endl;
                    if(sr->insertOrderToDB(odt))
                        sr->sendToClient(std::stoi(res[7]), res[1] + "|success");
                    else
                        sr->sendToClient(std::stoi(res[7]), res[1] + "|failed, repeat nid");
                }
                else
                {
                    logwrite->write(LogLevel::DEBUG, "(Trader) Insufficient balance");
                    sr->sendToClient(std::stoi(res[7]), res[1] + "|failed, Insufficient balance");
                }    
            }
            else
            {
                logwrite->write(LogLevel::DEBUG, "(Trader) Send Error report");
                sr->sendToClient(std::stoi(res[7]), res[1] + "|failed");
            }
        }
        else
            std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

void Trader::getCancelOrder()
{
    logwrite->write(LogLevel::DEBUG, "(Trader) Get Order Cancel/Modify thread start");
    while(getTraderStatus())
    {
        if(sr->dq_orderhandle->checkSpace()>0)
        {
            logwrite->write(LogLevel::DEBUG, "(Trader) Handle Order Cancel/Modify");
            std::string val = sr->dq_orderhandle->popDTA();
            std::vector<std::string> res = split(val, "|");
            if(res[0] == "88")
            {
                if(res[3] == "1")
                {
                    logwrite->write(LogLevel::DEBUG, "(Trader) Delete buy order");
                    std::string nid = res[1];
                    auto it = std::find_if(buyside_.begin(), buyside_.end(), [&](Order* obj) {return obj->nid == nid;});
                    if (it != buyside_.end())
                    {
                        logwrite->write(LogLevel::DEBUG, "(Trader) find target, try delete");
                        buyside_.erase(it);
                        sr->sendToClient(std::stoi(res[7]), res[1] + "|cancel order success");
                        db->updateOrderSituation(nid, "3");
                    }
                    else
                    {
                        logwrite->write(LogLevel::DEBUG, "(Trader) target not found");
                        sr->sendToClient(std::stoi(res[7]), res[1] + "|no this order");
                    }
                }
                else if(res[3] == "2")
                {
                    logwrite->write(LogLevel::DEBUG, "(Trader) Delete sell order");
                    std::string nid = res[1];
                    auto it = std::find_if(sellside_.begin(), sellside_.end(), [&](Order* obj) {return obj->nid == nid;});
                    if (it != sellside_.end())
                    {
                        logwrite->write(LogLevel::DEBUG, "(Trader) find target, try delete");
                        sellside_.erase(it);
                        sr->sendToClient(std::stoi(res[7]), res[1] + "|cancel order success");
                        db->updateOrderSituation(nid, "3");
                    }
                    else
                    {
                        logwrite->write(LogLevel::DEBUG, "(Trader) target not found");
                        sr->sendToClient(std::stoi(res[7]), res[1] + "|no this order");
                    }
                }
                else
                {
                    sr->sendToClient(std::stoi(res[7]), res[1] + "|cancel order error");
                }
            }
            else
            {
                logwrite->write(LogLevel::DEBUG, "(Trader) Handle modify order");
                std::string nid = res[1];
                auto it = std::find_if(buyside_.begin(), buyside_.end(), [&](Order* obj) {return obj->nid == nid;});
                if(res[3] == "1")
                {
                    if (it != buyside_.end())
                    {
                        logwrite->write(LogLevel::DEBUG, "(Trader) find target, try modify");
                        (*it)->orderPrice = std::stod(res[2]);
                        sr->sendToClient(std::stoi(res[7]), res[1] + "|modify order success");
                        db->updateOrderPrice(nid, res[2]);
                    }
                    else
                    {
                        logwrite->write(LogLevel::DEBUG, "(Trader) modify target not found");
                        sr->sendToClient(std::stoi(res[7]), res[1] + "|no this order, modify failed");
                    }
                }
                else
                {
                    if (it != sellside_.end())
                    {
                        logwrite->write(LogLevel::DEBUG, "(Trader) find target, try modify");
                        (*it)->orderPrice = std::stod(res[2]);
                        sr->sendToClient(std::stoi(res[7]), res[1] + "|modify order success");
                        db->updateOrderPrice(nid, res[2]);
                    }
                    else
                    {
                        logwrite->write(LogLevel::DEBUG, "(Trader) modify target not found");
                        sr->sendToClient(std::stoi(res[7]), res[1] + "|no this order, modify failed");
                    }
                }
            }
        }
        else
            std::this_thread::sleep_for(std::chrono::seconds(5));
    }

}

void Trader::sendExecReport(Order *order)
{
    sr->sendToClient(order->connId, order->nid + "|OrderExec");
    if(db->insertReport(order->nid, std::to_string(order->orderPrice), std::to_string(order->execPrice),order->getside()==Side::BUY?"1":"2", std::to_string(order->client_serialNum)))
        logwrite->write(LogLevel::DEBUG, "(Trader) Execution report send");
    db->updateOrderSituation(order->nid, "2");
}

void Trader::generateExecReport()
{
    while(getTraderStatus())
    {
        if(reportList_.size()>0)
        {
            logwrite->write(LogLevel::DEBUG, "(Trader) Handle Exec Order");
            std::shared_ptr<Order*> p = reportList_.back();
            sendExecReport(*p.get());
            p.reset();
            reportList_.pop_back();
        }
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

void Trader::endTransaction()
{
    serverstatus = true;
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
        std::thread cancelorderReceive(&Trader::getCancelOrder, this);
        std::thread matchup(&Trader::matchup, this);
        std::thread handleExecReport(&Trader::generateExecReport, this);
        cancelorderReceive.detach();
        matchup.detach();
        orderReceive.detach();
        handleExecReport.detach();
    }
    else
    {
        serverstatus = true;
    }

    std::unique_lock<std::mutex> lk1(cv_m);
    cv_.wait(lk1, [this]{return serverstatus;});
    
    
    
    
}