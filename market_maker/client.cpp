#include "client.h"

Client::Client(std::string initFilePath, std::string initchosen, std::string logPath)
{
    logwrite = new Logwriter("CL", logPath);
	ip = new InitParser(initFilePath, initchosen);
    allowConn();
    // std::thread sendtd(&Client::sendTypeMsg,this);
    std::thread recvtd(&Client::recvMsg,this);
    std::thread heatbeat(&Client::heartbeatSending, this);
    // sendtd.join();
    heatbeat.join();
    recvtd.detach();
};

void Client::setConnStatus(bool connStatus)
{
    std::lock_guard<std::mutex> lckConnect(mutex_);
	connStatus_ = connStatus;
}

bool Client::getConnStatus()
{
    std::lock_guard<std::mutex> lckConnect(mutex_);
	return connStatus_;
}

void Client::socketini()
{
    ip->readLine();
	const char* addr = ip->iniContainer["addr"].c_str();
	int port = std::stoi(ip->iniContainer["port"]);
    logwrite->write(LogLevel::DEBUG, "(Client) connect to " + ip->iniContainer["addr"] + " " + ip->iniContainer["port"]);
	
    if ((sockfd_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        logwrite->write(LogLevel::WARN, "(Client) socket initialise failed");
        return;
    }

    int addlen = sizeof(addr);
    serv_addr_.sin_family = AF_INET;
    serv_addr_.sin_port = htons(port);

    if(inet_pton(AF_INET, addr, &serv_addr_.sin_addr)<=0)  
    { 
        logwrite->write(LogLevel::WARN, "(Client) Invalid address/ Address not supported");
        setConnStatus(false);
        return; 
    }

    if (connect(sockfd_, (struct sockaddr *)&serv_addr_, sizeof(serv_addr_)) < 0) 
    { 
        logwrite->write(LogLevel::WARN, "(Client) Connection Failed");
        setConnStatus(false);
        return;
    }
    setConnStatus(true);
}

void Client::allowConn()
{
    while(true)
    {
        socketini();
        if (getConnStatus()) 
        {
            logwrite->write(LogLevel::DEBUG, "(Client) connect success ");
            firstconn_flag_ = false;
            cv_.notify_all();
            connCalculate = 0;
            break;
        }
        else if(firstconn_flag_)
        {
            logwrite->write(LogLevel::DEBUG, "(Client) Connect failed" );
            exit_flag_ = true;
            break;
        }
        else
        {
            logwrite->write(LogLevel::DEBUG, "(Client) try connect" );
            connCalculate++;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        
        if(connCalculate>3)
        {
            exit_flag_ = true;
            logwrite->write(LogLevel::DEBUG, "(Client) reconnect failed" );
            break;
        }
    }
}

void Client::sendMsg(std::string str)
{
    try
    {
        sendSignal_ = send(sockfd_, str.c_str(), recvbuflen_, 0);
        logwrite->write(LogLevel::DEBUG, "(Client) Send Message " + str);
        if(sendSignal_<0)
            throw "error";
    }
    catch(...)
    {
        logwrite->write(LogLevel::ERROR, "(Client) send failed ");
        std::unique_lock<std::mutex> lck2(mutex_);
        cv_.wait(lck2);
    }
}

void Client::sendTypeMsg()
{
    while(!exit_flag_)
    {
        std::string str;
        std::cin>>str;
        sendMsg(str.c_str());
    }
}

void Client::heartbeatSending()
{
    while(!exit_flag_)
    {
        sendMsg("<3&");
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}

void Client::recvMsg()
{
    while(!exit_flag_)
    {
        recvSignal_ = recv(sockfd_, buffer_, recvbuflen_, 0);
        if ( recvSignal_ > 0 )
        {
            buffer_[recvSignal_] = '\0';
            logwrite->write(LogLevel::DEBUG, "(Client) recv " + std::string(buffer_));
        }
        else
        {
            logwrite->write(LogLevel::ERROR, "(Client) recv failed");
            close(sockfd_);
            setConnStatus(false);
            allowConn();
        }
    }
}

int main()
{
    Client *cl = new Client("./doc/settings.ini", "TraderServer", "./log/");
}



