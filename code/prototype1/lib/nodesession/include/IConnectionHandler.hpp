#pragma once
#include "ISocket.hpp"
#include "IConnection.hpp"
#include "LiveMonitor.hpp"
#include "IQualityStrategy.hpp"
#include <functional>
#include "IStateContext.hpp"

class IConnectionHandler {
public:
	virtual IConnection::Status send(std::string const& packet, int flag = 0) const = 0;
	virtual IConnection::Status recv(std::string& packet, int flag = 0) const = 0;

	virtual IConnection::Status send(Packet& packet, int flag = 0) const = 0;
	virtual IConnection::Status recv(Packet& packet, int flag = 0) const = 0;

	//virtual void setContext(std::weak_ptr<IStateContext> ctx) = 0;
	virtual ~IConnectionHandler() = default;
	virtual uint32_t getId() const = 0;
};


/*
 * How i want this to look like:
 *
 * auto conn = socket.accept();
 *
 * auto newNode = std::make_shared<MonitoredNode>(...)
 *
 * //auto fabric = std::make_shared<SessionFabric>(sessionList);
 *
 * auto connHandler = std::make_shared<Handshaker>(
 *						conn, std::weak_ptr(newNode), fabric, dbComm);
 *
 * newNode->setConnectionHandler(connHandler);
 *
 */
