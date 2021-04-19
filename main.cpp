#include "main.h"

int main(int argc, char *argv[])
{
    // Server *sr = new Server("./doc/settings.ini", "socket", "./log/");
    std::string mode(argv[1]);
    Trader *td = new Trader(mode=="test"?true:false);
    td->startTransaction();
    return 0;
}