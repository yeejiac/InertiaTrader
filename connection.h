#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <string>
#include <algorithm>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>

#include "./funclib/initParser.h"
#include "./funclib/logwriter.h"

#define buffer 1024

class Connection
{
public:
    Connection(int new_socket, int id);
    Connection(const Connection& rhs);
    Connection& operator=(const Connection& rhs);
    void setRecvStatus(bool stat);
    bool getRecvStatus();
    bool recvfrom(std::string& str);
    void sendto(std::string str);
    int getConnectionID();
    bool login_flag = false;
private:
    int connection_socket_;
    int connectionID_;
    bool recvStatus_;
    char buffer_[buffer];
	int recvbuflen_ = buffer;
    mutable std::mutex mutex_;
    std::condition_variable conncv_;
};

#endif