#include <iostream>

class OrderData
{
public:
    std::string nid; //網單編號
    int volumn; //委託量
    double orderPrice; //委託價
    std::string symbol; //商品代號
    std::string userID; //委託人帳號
};

struct ReportData
{
    long nid; //網單編號
    std::string orderID; //委託單號
    double orderPrice; //委託價
    int orderVolumn; //委託量
    int execVolumn; //成交量
    std::string symbol; //商品代號
    std::string userID; //委託人帳號
};

struct UserData
{
    std::string userID;
    std::string password;
};