#ifndef CLIENT_H_
#define CLIENT_H_

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <string>
#include <algorithm>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

#include "../funclib/dataQueue.h"
#include "../funclib/initParser.h"
#include "../funclib/logwriter.h"

#define buffer 1024


class Client
{
public:
    Client(std::string initFilePath, std::string initchosen, std::string logPath);
    void socketini();
    void allowConn();
    void setConnStatus(bool stat);
    bool getConnStatus();
    void sendMsg(std::string str);
    void sendTypeMsg();
    void heartbeatSending();
    void recvMsg();
    void reConnect();
    Logwriter *logwrite;
    InitParser *ip;
	DataQueue *dq;
    int connCalculate = 0;
	
private:
    std::string configSelect_;
    bool exit_flag_ = false;
    bool firstconn_flag_ = true;
    bool connStatus_ = false;
    int sockfd_;
    struct sockaddr_in serv_addr_ ;
    struct sockaddr_in *ptr_ = NULL;
    int iniSignal_;
    int sendSignal_;
    int recvSignal_;
    char buffer_[buffer];
	int recvbuflen_ = buffer;
    std::mutex mutex_;
    std::condition_variable cv_;
};
#endif