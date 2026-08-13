#pragma once
#include "ISocket.hpp"
#include "TcpCaller.hpp"
#include "TcpCallee.hpp"

#include "ILogger.hpp"
#include "ConsoleLogger.hpp"


class TcpSocket: public ISocket {
	int sockFd;
	struct sockaddr_storage addr;
	int port;

	void initAddress(std::string const& address, int port);

public:
	// OMG RESOLVER 
	TcpSocket(int sockfd, std::string const& address, int port); 

	std::unique_ptr<IConnection> accept() const override;
	std::unique_ptr<IConnection> connect(struct sockaddr_storage addr) const override;

	void bind() const override;
	void listen(int backlog) const override;

	int getDescriptor() const override;

	~TcpSocket();
};
