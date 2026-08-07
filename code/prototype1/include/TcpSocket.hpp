#pragma once
#include "ISocket.hpp"

class TcpSocket: public ISocket {
	int sockFd;
	std::string addr;
	int port;

protected:
	std::unique_ptr<IConnection> createCallee() const; 
	std::unique_ptr<IConnection> createCaller() const; 

public:
	// OMG RESOLVER 
	TcpSocket(std::string const& addr, int port); 
	TcpSocket(int sockfd, std::string const& addr, int port); 

	std::unique_ptr<IConnection> accept() const override;
	std::unique_ptr<IConnection> connect() const override;

	void bind() const override;
	int listen(int backlog) const override;

	int getDescriptor() const override;

	~TcpSocket();
};

	
