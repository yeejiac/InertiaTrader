#include "main.h"

int main()
{
    Server *sr = new Server("./doc/settings.ini", "socket", "./log/");
    return 0;
}