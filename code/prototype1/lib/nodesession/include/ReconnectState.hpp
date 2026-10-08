#pragma once 
#include "IConnectionHandler.hpp"
#include "Handshaker.hpp"
#include "IStateContext.hpp"


#define MSGTYPE_RECONNECT	(uint8_t)(0x7 << MsgType_Pos)
#define RECONNECT_HEADERLEN 0x3

class ReconnectState: public IConnectionHandler {
	std::unique_ptr<IConnection> connection;
	std::weak_ptr<IStateContext> context;
	std::shared_ptr<NodeRegistry> registry;
	std::shared_ptr<IDataAccessor> storage;

	uint16_t parseMsg(Packet const& packet) const; // -> sessionId
public:
	ReconnectState(std::unique_ptr<IConnection> conn, 
			std::shared_ptr<IStateContext> ctx,
			std::shared_ptr<NodeRegistry> reg, 
			std::shared_ptr<IDataAccessor> stor);

	IConnection::Status recv(Packet& packet, int flag = 0) override;
	IConnection::Status send(Packet& packet, int flag = 0) const override;

	IConnection::Status recv(std::string& message, int flag = 0) override;
	IConnection::Status send(std::string const& message, int flag = 0) const override;

	void setConnection(std::unique_ptr<IConnection> conn) override;
	std::unique_ptr<IConnection> getConnection();
	void setContext(std::shared_ptr<IStateContext> ctx) override;
	int getDescriptor() const override;
};

