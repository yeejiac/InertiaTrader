#include "dataQueue.h"

DataQueue::DataQueue(int limitation):limitation_(limitation){}

void DataQueue::pushDTA(std::string val)
{
    std::lock_guard<std::mutex> lck(mutex_);
    dataQueue_.push(val);
    logwrite.write(LogLevel::DEBUG, "(DataQueue) get data: "+val);
    cv_.notify_one();
}

std::string DataQueue::popDTA()
{
    std::unique_lock<std::mutex> lck(mutex_);
    cv_.wait_for(lck, std::chrono::seconds(1), [this] { return !dataQueue_.empty(); });
    std::string val = dataQueue_.back();
    dataQueue_.pop();
    logwrite.write(LogLevel::DEBUG, "(DataQueue) pop data: "+val);
    return val;
}

int DataQueue::checkSpace()
{
    return dataQueue_.size();
}

// int main()
// {
//     DataQueue *dq = new DataQueue(100);
//     dq->pushDTA("123");
//     std::cout<<dq->popDTA()<<std::endl;
// }