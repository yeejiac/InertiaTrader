#include "dataQueue.h"

DataQueue::DataQueue(int limitation):limitation_(limitation){}

void DataQueue::pushDTA(std::string val)
{
    std::unique_lock<std::mutex> lck(mutex_);
    dataQueue_.push(val);
    logwrite.write(LogLevel::DEBUG, "(DataQueue) get data: "+val);
    // cv_.wait_for(lck, []{return this.checkSpace() < &limitation_;} );
    lck.unlock();
}

std::string DataQueue::popDTA()
{
    std::unique_lock<std::mutex> lck(mutex_);
    std::string val = dataQueue_.back();
    dataQueue_.pop();
    lck.unlock();
    return dataQueue_.back();
}

int DataQueue::checkSpace()
{
    return dataQueue_.size();
}