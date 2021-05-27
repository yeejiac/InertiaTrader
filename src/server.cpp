#include "server.h"

Server::Server(std::string initFilePath, std::string initchosen, std::string logPath, bool mode):initFilePath(initFilePath),
initchosen(initchosen), mode_(mode),logPath(logPath)
{
	logwrite = new Logwriter("SR", logPath);
	essentialData_initialise();
	if(db->connstatus&&socketini())
	{
		std::thread connacpt(&Server::acceptConn,this);
		connacpt.detach();
	}
	else
		setconnStatus(false);
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

bool Server::socketini()
{
	std::unique_ptr<InitParser> ip(new InitParser(initFilePath, initchosen));
	ip->readLine();
	int port = std::stoi(ip->iniContainer["port"]);
	const char* addr = ip->iniContainer["addr"].c_str();
	if( (listenfd_ = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	{
        printf("create socket error: %s(errno: %d)\n");
        return false;
    }
	memset(&servaddr_, 0,sizeof(servaddr_));
	servaddr_.sin_family = AF_INET;
	servaddr_.sin_port = htons(port);
	
    if (bind( listenfd_, (struct sockaddr*)&servaddr_, sizeof(servaddr_)) == -1) 
	{
		logwrite->write(LogLevel::WARN, "bind failed with Error");
        return false;
    }

	
    if (listen( listenfd_, SOMAXCONN) == -1) 
	{
		logwrite->write(LogLevel::WARN, "listen failed with Error: ");
		return false;
	}
	setconnStatus(true);
	logwrite->write(LogLevel::DEBUG, "server socket init success");
	return true;
}

void Server::essentialData_initialise()
{
	db = new TradingDataHandler(mode_?"Testdatabase":"database");
	if(db->connstatus)
		userList = db->getUserData();
	else
		logwrite->write(LogLevel::ERROR, "(Server) Connect to DB failed");

	logwrite->write(LogLevel::ERROR, "(Server) Load price status");
	st = new Stock;
	st->priceMax = 60;
	st->priceMin = 60;
	st->priceNow = 60;
	quotesList.insert(std::pair<std::string, Stock*>("KKC", st));
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
		cn->sendto(hearbeatString + "\n");
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
		logwrite->write(LogLevel::DEBUG, "Recv row msg : " + recvStr);
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
			{
				std::vector<std::string> tempVec = split(subStr, "&");
				for(int i = 0; i < tempVec.size(); i++)
				{
					msgHandler(tempVec[i] + "|" + std::to_string(cn->getConnectionID()) + "|");
				}
			}
			else
				loginMsgHandle(subStr, cn);
		}
		else
		{
			freeEmptysocket();
			break;
		}
	}
}

void Server::msgHandler(std::string msg)
{
	if(msg.substr(0,2) == "<3")
		return;
	std::string codeNum = msg.substr(0, msg.find("|"));
	try
	{
		switch(std::stoi(codeNum))
		{
			case 87:
				logwrite->write(LogLevel::DEBUG, "(Server) 收到下單電文");
				//進行基本委託檢核(帳戶餘額檢查)
				dq->pushDTA(msg);
				break;
			case 88:
			case 89:
				logwrite->write(LogLevel::DEBUG, "(Server) 收到刪單或改單電文");
				dq_orderhandle->pushDTA(msg);
			case 1233:
				try
				{
					logwrite->write(LogLevel::DEBUG, "(Server) recv price asking");
					std::vector<std::string> temp = split(msg, "|");
					sendToClient(std::stoi(temp[2]), "1233|"+std::to_string(quotesList[temp[1]]->priceNow)+"|&");
				}
				catch(const std::exception& e)
				{
					std::cerr << e.what() << '\n';
				}
			default:
				break;
		}
	}
	catch(...)
	{
		logwrite->write(LogLevel::DEBUG, "(Server) Handle Msg error ");
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
				std::map<std::string, UserData*>::iterator it = userList.find(res[1]);
				if(it->second->password == res[2])
				{
					logwrite->write(LogLevel::DEBUG, "(Server) login success");
					cn->sendto("login|" + std::to_string(rand()%100000) + "\n");
					cn->setloginFlag(true);
					cn->username = res[1];
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
	else if(msg.substr(0,4) == "1238")
	{
		try
		{
			logwrite->write(LogLevel::DEBUG, "(Server) Add new user");
			std::vector<std::string> res = split(msg, "|");
			UserData *ud = new UserData;
			ud->userID = res[1];
			ud->password = res[2];
			ud->balance = std::stod(res[3]);
			ud->book_value = std::stod(res[4]);
			if(db->addTrader(ud))
			{
				std::pair<std::string, UserData*> temp(res[1], std::move(ud));
				userList.insert(temp);
			}
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
}

bool Server::insertOrderToDB(OrderData *od)
{
	if(db->insertOrder(od))
	{
		logwrite->write(LogLevel::DEBUG, "(Server) Sent to db success");
		return true;
	}
	else
	{
		logwrite->write(LogLevel::DEBUG, "(Server) Sent to db failed");
		return false;
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

void Server::sendToClient(int connNum, std::string msg)
{
	logwrite->write(LogLevel::DEBUG, "(Server) Send to client " + std::to_string(connNum));
	connStorage_[connNum]->sendto(msg + "\n");
}

Connection* Server::getConnObject(int connNum)
{
	Connection *cn = connStorage_[connNum];
	std::cout<<cn->username<<std::endl;
	return cn;
}

void Server::quoteUpdate(std::string stockNum, double execPrice)
{
	try
	{
		quotesList[stockNum]->priceNow = execPrice;
		if(quotesList[stockNum]->priceMin>execPrice)
		{
			quotesList[stockNum]->priceMin = execPrice;
		}
		else if(quotesList[stockNum]->priceMax<execPrice)
		{
			quotesList[stockNum]->priceMax = execPrice;
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}

// int main()
// {
// 	Server *sr = new Server;
// }

