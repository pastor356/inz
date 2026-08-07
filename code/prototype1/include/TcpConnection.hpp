#pragma once
#include "IConnection.hpp"

class TcpConnection: public IConnection {
	int connectionFd;
	std::string addr;
	int port; // in clients case -- getnameinfo()

public:
	TcpConnection(int sockFd, std::string const& addr, int port);

	Status recv(std::string& msg, int flag = 0) const override;
	void send(std::string const& msg, int flag = 0) const override;
	const std::string& getAddr() const override;

	int getDescriptor() const; 
};

	
