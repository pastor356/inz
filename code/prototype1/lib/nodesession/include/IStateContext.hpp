#pragma once
#include "ISocket.hpp"
#include "IConnection.hpp"
#include "LiveMonitor.hpp"
#include "IConnectionHandler.hpp"
#include "IQualityStrategy.hpp"
#include "IDataAccessor.hpp"
#include <functional>
#include <memory>

class IConnectionHandler;

class IStateContext {
public:
	virtual void changeState(std::shared_ptr<IConnectionHandler> newState) = 0;

	virtual void changeStateHandshake(std::unique_ptr<IConnection> conn,
			std::shared_ptr<IStateContext> ctx) = 0;

	virtual void changeStateReconnect(std::unique_ptr<IConnection> conn,
			std::shared_ptr<IStateContext> ctx) = 0;

	virtual void changeStateSession(std::unique_ptr<IConnection> conn,
			std::shared_ptr<IStateContext> ctx, UserAuth authUser) = 0;


	virtual IConnectionHandler* getHandler() const = 0;

	virtual IConnection::Status recv(Packet& packet, int flag = 0) = 0;
	virtual IConnection::Status send(Packet& packet, int flag = 0) = 0;
};
