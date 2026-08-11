#pragma once
#include "IConnection.hpp"

class TcpConnection: public IConnection {
	int connectionFd;
	struct sockaddr_storage addr;

	std::string sock_ntop() const; 

public:
	TcpConnection(int sockFd, std::string const& addr, int port);
	TcpConnection(int sockFd, struct sockaddr_storage ipAddr);
	virtual ~TcpConnection();

	virtual IConnection::Status recv(std::string& msg, int flag = 0) const override;
	virtual void send(std::string const& msg, int flag = 0) const override;
	virtual const struct sockaddr_storage getAddr() const override;

	int getDescriptor() const; 
};

