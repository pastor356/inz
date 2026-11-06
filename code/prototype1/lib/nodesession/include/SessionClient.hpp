#pragma once
#include "IConnection.hpp"
#include "Session.hpp"

class SessionClient {
	std::unique_ptr<IConnection> connection;
	uint16_t sessionId;

public:
	SessionClient(std::unique_ptr<IConnection> conn);

	IConnection::Status recv(Packet& packet, int flag = 0); 
	IConnection::Status send(Packet& packet, int flag = 0);

	int getDescriptor() const;
};

