#include "main.h"

int main()
{
    // Server *sr = new Server("./doc/settings.ini", "socket", "./log/");
    Trader *td = new Trader;
    td->startTransaction();
    return 0;
}