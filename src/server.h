#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <algorithm>
#include <mutex>
#include <condition_variable>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <thread>
#include <map>
#include <memory>
#include "../funclib/dataQueue.h"
#include "../funclib/initParser.h"
#include "../funclib/logwriter.h"
#include "../funclib/simplefunc.h"
#include "../database/tradingDataHandler.h"

#include "connection.h"


#define buffer 1024

class Server
{
public:
	Server(std::string initFilePath, std::string initchosen, std::string logPath);
	~Server();
	void socketini();
	void essentialData_initialise();
	void acceptConn();
	void msgRecv(Connection *cn);
	void msgHandler(std::string msg);
	void send(Connection *cn);
	void heartbeat(Connection *cn);
	void setconnStatus(bool connStatus);
	bool getconnStatus();
	void freeEmptysocket();
	void loginMsgHandle(std::string msg, Connection *cn);
	Logwriter *logwrite;
	DataQueue *dq = new DataQueue(10);
	TradingDataHandler *db;
	std::string initFilePath;
	std::string initchosen;
	std::string logPath;
	std::map<std::string, std::string> userList;
private:
	int listenfd_;
	int connfd_;
	struct sockaddr_in servaddr_;
	int iSendResult_;
	char buffer_[buffer];
	int recvbuflen_ = buffer;
	bool connStatus_;
	std::mutex mutex_;
	std::condition_variable st_;
    std::condition_variable conncv_;
	std::condition_variable limitLock_;
	std::map<int, Connection*> connStorage_;
	
};



#endif