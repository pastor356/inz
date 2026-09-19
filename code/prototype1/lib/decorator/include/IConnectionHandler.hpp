#pragma once
#include "Session.hpp"
#include "ISocket.hpp"
#include "IConnection.hpp"
#include "LiveMonitor.hpp"
#include "IQualityStrategy.hpp"
#include "ConsoleLogger.hpp"

class IConnectionHandler {
public:
	IConnection::Status recv(Packet& packet) const = 0;
	IConnection::Status send(Packet const& packet) const = 0;
};
