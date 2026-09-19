#pragma once
#include "ISocket.hpp"
#include "IConnection.hpp"
#include "LiveMonitor.hpp"
#include "IQualityStrategy.hpp"
#include <functional>

class IConnectionHandler {
public:
	virtual IConnection::Status send(Packet& packet, int flag = 0) const = 0;
	virtual IConnection::Status recv(Packet& packet, int flag = 0) const = 0;
	virtual void setContext(std::shared_ptr<IStateContext> ctx) = 0;
	virtual ~IConnectionHandler() = default;
};


