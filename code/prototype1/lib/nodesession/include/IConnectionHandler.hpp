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
	virtual IConnection::Status recv(std::string& packet, int flag = 0) = 0;

	virtual IConnection::Status send(Packet& packet, int flag = 0) const = 0;
	virtual IConnection::Status recv(Packet& packet, int flag = 0) = 0;

	virtual int getDescriptor() const = 0;
	virtual void setConnection(std::unique_ptr<IConnection> conn) = 0;
	virtual std::unique_ptr<IConnection> getConnection() = 0;
	virtual void setContext(std::shared_ptr<IStateContext> ctx) = 0;

	virtual ~IConnectionHandler() = default;
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
