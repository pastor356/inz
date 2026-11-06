#pragma once
#include "IConnection.hpp"

#include "ILogger.hpp"
#include "ConsoleLogger.hpp"

class TcpConnection: public IConnection {
	int connectionFd;
	struct sockaddr_storage addr;

	std::string sock_ntop() const; 

	IConnection::Status sendHeader(std::vector<uint8_t> const& header) const;
	IConnection::Status sendPayload(std::string const& payload, bool needSend = true) const;
	IConnection::Status recvHeader(std::vector<uint8_t>& header, int flag = 0) const;
	IConnection::Status recvPayload(std::string& payload, bool expected = true, int flag = 0) const;

public:
	TcpConnection(int sockFd, std::string const& addr, int port);
	TcpConnection(int sockFd, struct sockaddr_storage ipAddr);
	virtual ~TcpConnection();

	virtual IConnection::Status recv(std::string& msg, size_t nbytes, int flag = 0) const;
	virtual IConnection::Status send(std::string const& msg, size_t nbytes, int flag = 0) const;

	virtual IConnection::Status recv(std::string& msg, int flag = 0) const override;
	virtual IConnection::Status send(std::string const& msg, int flag = 0) const override;

	virtual Status recv(Packet& packet, int flag = 0) const override;
	virtual Status send(Packet& packet, int flag = 0) const override;

	virtual const struct sockaddr_storage getAddr() const override;

	int getDescriptor() const override; 
};

