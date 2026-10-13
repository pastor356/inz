#pragma once
#include "IStateContext.hpp"
#include "IConnectionHandler.hpp"
#include "IDataAccessor.hpp"
#include "NodeRegistry.hpp"
#include "ReconnectState.hpp"
#include "TerminatedState.hpp"


class MonitoredNode: public IStateContext {
	std::shared_ptr<IConnectionHandler> connection;
	std::shared_ptr<IConnectionHandler> oldConn;

	std::shared_ptr<IDataAccessor> storage;
	std::shared_ptr<NodeRegistry> registry;

public:
	MonitoredNode() = default;
	MonitoredNode(std::shared_ptr<IConnectionHandler> initial,
			std::shared_ptr<IDataAccessor> storage, std::shared_ptr<NodeRegistry> registry);

	void changeState(std::shared_ptr<IConnectionHandler> newState) override;

	IConnectionHandler* getHandler() const override;
	
	void changeStateHandshake(std::unique_ptr<IConnection> conn,
			std::shared_ptr<IStateContext> ctx) override;

	void changeStateReconnect(std::unique_ptr<IConnection> conn,
			std::shared_ptr<IStateContext> ctx) override;

	void changeStateSession(std::unique_ptr<IConnection> conn,
			std::shared_ptr<IStateContext> ctx, UserAuth authUser) override;

	void changeStateTerminated(std::shared_ptr<IStateContext> ctx, 
			UserAuth authUser) override;

	IConnection::Status recv(Packet& packet, int flag = 0) override;
	IConnection::Status send(Packet& packet, int flag = 0) override;
};
