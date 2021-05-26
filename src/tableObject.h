#ifndef TABLEOBJECT_H_
#define TABLEOBJECT_H_
#include <iostream>

class Stock
{
public:
    Stock();
    ~Stock();
    int stockNum;
    double priceNow;
    double priceMax;
    double priceMin;
};

#endif