#pragma once
#include <memory>
#include "IConnection.hpp"
#include "Packet.hpp"
#include "ITimerQueue.hpp"


enum class PacketType {
	Qos1, 
	Qos2,
	KeepAlive
};

class ConnectionDecorator: public IConnection {
protected:
	std::unique_ptr<IConnection> connection;
	std::shared_ptr<ITimerQueue> timer;

public:
	IConnection::Status send(std::string const& msg, int flag = 0) const override;
	IConnection::Status recv(std::string& msg, int flag = 0) const override;
	const struct sockaddr_storage getAddr() const override;
	int getDescriptor() const override;

	IConnection::Status send(Packet& packet, int flag = 0) const override;
	IConnection::Status recv(Packet& packet, int flag = 0) const override;

	ConnectionDecorator(std::unique_ptr<IConnection> conn, std::shared_ptr<ITimerQueue> timer);
};


