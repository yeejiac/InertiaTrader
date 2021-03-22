#include "connection.h"

Connection::Connection(int new_socket, int id):connection_socket_(new_socket), connectionID_(id), recvStatus_(true){}

Connection::Connection(const Connection& rhs)
{
    connection_socket_ = rhs.connection_socket_;
    recvStatus_ = rhs.recvStatus_;
}

Connection& Connection::operator=(const Connection& rhs)
{
    if(this != &rhs)
    {
        connection_socket_ = rhs.connection_socket_;
        recvStatus_ = rhs.recvStatus_;
    }
    return *this;
}

void Connection::setRecvStatus(bool stat)
{
    std::lock_guard<std::mutex> lckConnect(mutex_);
    recvStatus_ = stat;
    conncv_.notify_all();

}

bool Connection::getRecvStatus()
{
    std::lock_guard<std::mutex> lckConnect(mutex_);
    return recvStatus_;
}

bool Connection::recvfrom(std::string& str)
{
    int recvStatus = recv(connection_socket_, buffer_, recvbuflen_, 0);
    if(recvStatus>0)
    {
        buffer_[recvStatus] = '\0';
        str = std::move(buffer_);
    }
    else
    {
        setRecvStatus(false);
    }
    return getRecvStatus();
}

void Connection::sendto(std::string str)
{
    int sendStauts = send(connection_socket_, str.c_str(), recvbuflen_, 0);
    if(sendStauts<0)
    {
        setRecvStatus(false);
    }
}

int Connection::getConnectionID()
{
    return connectionID_;
}