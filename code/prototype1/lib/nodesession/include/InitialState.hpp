#pragma once
#include "IConnectionHandler.hpp"


class InitialState: public IConnectionHandler {
	std::unique_ptr<IConnection> connection;
	std::weak_ptr<IStateContext> context;

public:
	InitialState(std::unique_ptr<IConnection> conn, std::shared_ptr<IStateContext> ctx);

	IConnection::Status recv(Packet& packet, int flag = 0) override;
	IConnection::Status send(Packet& packet, int flag = 0) const override;

	IConnection::Status send(std::string const& packet, int flag = 0) const;
	IConnection::Status recv(std::string& packet, int flag = 0);

	void setConnection(std::unique_ptr<IConnection> conn) override;
	void setContext(std::shared_ptr<IStateContext> ctx) override;
	int getDescriptor() const override;
};
