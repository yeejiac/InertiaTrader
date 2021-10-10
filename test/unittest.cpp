#include "unittest.h"

void UnitTest::testTrader()
{
    Order *testorder;
    testorder->nid = "test123";
    testorder->setside(Side::BUY);
    testorder->orderPrice = 1000; //***Need to set the variable orderPrice to private level
    Trader *testTrader = new Trader(true);

    testTrader->essentialData_initialise(); //Initalise database object and parse basic trading information from database

    testTrader->loadExistOrder(); //Load exist order (Only run in restart)

    testTrader->orderDataInsert(testorder);



}

int main()
{
    return 0;
}