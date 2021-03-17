  
#ifndef DATAQUEUE_H_
#define DATAQUEUE_H_

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>

#include "logwriter.h"

class DataQueue
{
public:
    DataQueue(int limitation);
    void pushDTA(std::string val);
    std::string popDTA();
    int findVal(std::string);
    int checkSpace();
    Logwriter logwrite = Logwriter("FD", "../doc/log/");
private:
    int limitation_;
    std::queue<std::string> dataQueue_;
    std::condition_variable cv_;
    std::mutex mutex_;
};


#endif