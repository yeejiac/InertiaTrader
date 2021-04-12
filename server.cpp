#include "server.h"

Server::Server(std::string initFilePath, std::string initchosen, std::string logPath):initFilePath(initFilePath),initchosen(initchosen),
logPath(logPath)
{
	logwrite = new Logwriter("SR", logPath);
	essentialData_initialise();
	if(db->connstatus)
	{
		socketini();
		std::thread connacpt(&Server::acceptConn,this);
		connacpt.join();
	}
}

Server::~Server()
{
	close(connfd_);
	db->~TradingDataHandler();
}

void Server::setconnStatus(bool connStatus)
{
	connStatus_ = connStatus;
}

bool Server::getconnStatus()
{
	return connStatus_;
}

void Server::socketini()
{
	std::unique_ptr<InitParser> ip(new InitParser(initFilePath, initchosen));
	ip->readLine();
	int port = std::stoi(ip->iniContainer["port"]);
	std::cout<<port<<std::endl;
	const char* addr = ip->iniContainer["addr"].c_str();
	std::cout<<addr<<std::endl;
	if( (listenfd_ = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	{
        printf("create socket error: %s(errno: %d)\n");
        return;
    }
	memset(&servaddr_, 0,sizeof(servaddr_));
	servaddr_.sin_family = AF_INET;
	servaddr_.sin_port = htons(port);
	
    if (bind( listenfd_, (struct sockaddr*)&servaddr_, sizeof(servaddr_)) == -1) 
	{
		logwrite->write(LogLevel::WARN, "bind failed with Error");
        return;
    }

	
    if (listen( listenfd_, SOMAXCONN) == -1) 
	{
		logwrite->write(LogLevel::WARN, "listen failed with Error: ");
		return;
	}
	setconnStatus(true);
	logwrite->write(LogLevel::DEBUG, "server socket init success");
}

void Server::essentialData_initialise()
{
	db = new TradingDataHandler("database");
	userList = db->getUserData();
}

void Server::acceptConn()
{
	while(getconnStatus())
	{
		connfd_ = accept(listenfd_, (struct sockaddr *)NULL, NULL);
		if(connfd_ == -1)
		{
			logwrite->write(LogLevel::WARN, "accept failed with Error: ");
			continue;
		}
		else
		{
			setconnStatus(true);
			std::string connPortNum;

			std::unique_lock<std::mutex> lck3(mutex_);
			int space = rand()%99;
			Connection *cn = new Connection(connfd_, space);
			std::pair<int, Connection*> tmp(space, cn);
			connStorage_.insert(tmp);
			lck3.unlock();
			logwrite->write(LogLevel::DEBUG, "get socket connection: "+std::to_string(space));
			
			std::thread recvConn(&Server::msgRecv, this, cn);
			std::thread sendheartbeat(&Server::heartbeat, this, cn);
			recvConn.detach();
			sendheartbeat.detach();
		}
	}
}

void Server::heartbeat(Connection *cn)
{
	while(connStatus_)
	{
		std::string hearbeatString = "<3&";
		cn->sendto(hearbeatString);
		logwrite->write(LogLevel::DEBUG, "Send heartbeat");
		if(!cn->getRecvStatus())
		{
			freeEmptysocket();
			break;
		}
		std::this_thread::sleep_for(std::chrono::seconds(10));
	}
}

void Server::msgRecv(Connection *cn)
{
	std::string recvStr;
	std::string subStr;
	std::string tempStr = "";
	while(connStatus_)
	{
		bool connStatus = true;
		connStatus = cn->recvfrom(recvStr);
		if(connStatus)
		{
			if(tempStr!=""&&recvStr!="<3&")
			{
				recvStr = tempStr + recvStr;
				tempStr = "";
			}

			if(recvStr.back()!='&')
			{
				std::size_t found = recvStr.find_last_of('&');
				subStr = recvStr.substr(0, found);
				tempStr = recvStr.substr(found+1);
				logwrite->write(LogLevel::DEBUG, "Temp String : " + tempStr);
			}
			else
			{
				subStr = recvStr.substr(0, recvStr.length()-1);
			}
			logwrite->write(LogLevel::DEBUG, " Server Receive : " + subStr);
			if(cn->getloginstatus())
				msgHandler(subStr);
			else
				loginMsgHandle(subStr, cn);
			
			//dq->pushDTA(subStr + ":" + std::to_string(cn->getConnectionID())); //測試用
			if(cn->getloginstatus()&&subStr!="<3")
				dq->pushDTA(subStr + ":" + std::to_string(cn->getConnectionID()));
			else
			{
				if(subStr != "<3")
					cn->sendto("not log in");
			}	
		}
		else
		{
			st_.notify_one();
			freeEmptysocket();
		}
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
}

void Server::msgHandler(std::string msg)
{
	if(msg == "<3")
		return;
	std::vector<std::string> res = split(msg, "|");
	switch(std::stoi(res[0]))
	{
		case 87:
			logwrite->write(LogLevel::DEBUG, "(Server) 下單處理");
			OrderData *od;
			od->nid = std::stol(res[1]);
			od->orderPrice = std::stoi(res[2]);
			od->symbol = "TXO";
			od->userID = "0324027";

			if(db->insertOrder(od))
				logwrite->write(LogLevel::DEBUG, "(Server) Sent to db success");
			else
				logwrite->write(LogLevel::DEBUG, "(Server) Sent to db failed");
			break;
		default:
			break;
		
	}
}

void Server::loginMsgHandle(std::string msg, Connection *cn)
{
	if(msg.substr(0,4) == "1234")
	{
		std::vector<std::string> res = split(msg, "|");
		logwrite->write(LogLevel::DEBUG, "(Server) login verify");
		try
		{
			if(userList.size()>0)
			{
				std::map<std::string, std::string>::iterator it = userList.find(res[1]);
				std::cout<<it->second<<std::endl;
				std::cout<<res[2]<<std::endl;
				if(it->second == res[2])
				{
					logwrite->write(LogLevel::DEBUG, "(Server) login success");
					cn->setloginFlag(true);
				}	
			}
			else
			{
				std::cout<<"No data"<<std::endl;
			}
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
}

void Server::freeEmptysocket()
{
	std::map<int, Connection*>::iterator it;
	for(it = connStorage_.begin(); it!=connStorage_.end(); ++it)
	{
		if(!it->second->getRecvStatus())
		{
			std::cout<<it->first<<std::endl;
			logwrite->write(LogLevel::DEBUG, "(Server): free empty socket " + std::to_string(it->first));
			std::unique_lock<std::mutex> lckerase(mutex_);
			it = connStorage_.erase(it);
			lckerase.unlock();
		}
	}
}

// int main()
// {
// 	Server *sr = new Server;
// }

