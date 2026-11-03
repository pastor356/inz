#pragma once
#include "ISocket.hpp"
#include "TcpCaller.hpp"
#include "TcpCallee.hpp"
#include "Resolver.hpp"
#include "ConsoleLogger.hpp"

#include <arpa/inet.h>

class TcpSocket: public ISocket {
	int sockFd;
	struct sockaddr_storage addr;

	struct sockaddr_storage initAddress(std::string const& address, int port);

	std::string sock_ntop() const;
	std::string sock_ntop(struct addrinto* ai) const;

public:
	// setsockopt() method
	TcpSocket(int sockfd, std::string const& address, int port); 
	TcpSocket(std::string const& address, int port); 
	TcpSocket(addrInfo && resolvedAddress);

	std::unique_ptr<IConnection> accept() const override;
	
	// connect(const struct sockaddr* addr) overloading
	std::unique_ptr<IConnection> connect(struct sockaddr_storage addr) const override;
	std::unique_ptr<IConnection> connect(addrInfo && resolvedAddress) const override;

	void bind() const override;
	void listen(int backlog) const override;

	int getDescriptor() const override;

	~TcpSocket();
};
