#pragma once
#include "IConnectionHandler.hpp"
#include <vector>


class TerminatedState: public IConnectionHandler {
	std::weak_ptr<IStateContext> context;
	UserAuth auth;
	mutable std::vector<Packet> buffer;

public:
	TerminatedState(std::shared_ptr<IStateContext> ctx, UserAuth const& authUser);

	IConnection::Status send(std::string const& message, int flag = 0) const override;
	IConnection::Status recv(std::string& message, int flag = 0) override;

	IConnection::Status send(Packet& packet, int flag = 0) const override;
	IConnection::Status recv(Packet& packet, int flag = 0) override;

	int getDescriptor() const override;
	void setConnection(std::unique_ptr<IConnection> conn) override;
	std::unique_ptr<IConnection> getConnection() override;
	void setContext(std::shared_ptr<IStateContext> ctx) override;
};

