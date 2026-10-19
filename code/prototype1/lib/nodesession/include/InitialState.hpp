#pragma once
#include "IConnectionHandler.hpp"
#include "Handshaker.hpp"
#include "ReconnectState.hpp"


class InitialState: public IConnectionHandler {
	std::unique_ptr<IConnection> connection;
	std::weak_ptr<IStateContext> context;

	bool isHandshake(Packet const& packet) const;
	bool isReconnect(Packet const& packet) const;
public:
	InitialState(std::unique_ptr<IConnection> conn, std::shared_ptr<IStateContext> ctx);
	InitialState(std::unique_ptr<IConnection> conn);

	IConnection::Status recv(Packet& packet, int flag = 0) override;
	IConnection::Status send(Packet& packet, int flag = 0) const override;

	IConnection::Status send(std::string const& message, int flag = 0) const;
	IConnection::Status recv(std::string& message, int flag = 0);

	void setConnection(std::unique_ptr<IConnection> conn) override;
	std::unique_ptr<IConnection> getConnection() override;
	void setContext(std::shared_ptr<IStateContext> ctx) override;
	int getDescriptor() const override;
};
