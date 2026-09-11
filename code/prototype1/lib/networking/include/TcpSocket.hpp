#pragma once
#include "ISocket.hpp"
#include "TcpCaller.hpp"
#include "TcpCallee.hpp"
#include "Resolver.hpp"
#include "ConsoleLogger.hpp"

struct Connector {
public:
	// connect operation
	int operator() (int sockfd, struct sockaddr* addr, socklen_t addrlen) const; 
};


class TcpSocket: public ISocket {
	int sockFd;
	struct sockaddr_storage addr;

	struct sockaddr_storage initAddress(std::string const& address, int port);

	std::string sock_ntop() const;
public:
	// setsockopt() method
	TcpSocket(int sockfd, std::string const& address, int port); 
	TcpSocket(std::string const& address, int port); 
	TcpSocket(addrInfo && resolvedAddress);

	std::unique_ptr<IConnection> accept() const override;
	
	// connect(const struct sockaddr* addr) overloading
	std::unique_ptr<IConnection> connect(struct sockaddr_storage addr) const override;
	std::unique_ptr<IConnection> connect(addrInfo && resolvedAddress) const override;
	std::unique_ptr<IConnection> connect(addrInfo && resolvedAddress, Connector& connector) const;

	void bind() const override;
	void listen(int backlog) const override;

	int getDescriptor() const override;

	~TcpSocket();
};
