#pragma once
#include "IConnectionHandler.hpp"
#include "IStateFactory.hpp"
#include "Session.hpp"
#include "DatabaseCommunicator.hpp"
#include "SessionRegistry.hpp"
#include "SessionConfig.hpp"
#include "SessionBuilder.hpp"


#ifndef MsgType_Pos
#define MsgType_Pos			0x4
#endif

#define RoleType_Pos		0x2
#define QosType_Pos			0x0

// second byte
#define CREATFLAG_Pos		0x5
#define NEWUSRFLAG_Pos		0x4
#define RIGHTS_Pos			0x2

#define MSGTYPE_HANDSHAKE	(uint8_t)(0x06 << MsgType_Pos)
#define MSGTYPE_RECONNECT	(uint8_t)(0x07 << MsgType_Pos)
#define QOSTYPE_0			(uint8_t)(0x0 << QosType_Pos)
#define QOSTYPE_1			(uint8_t)(0x1 << QosType_Pos)
#define QOSTYPE_2			(uint8_t)(0x2 << QosType_Pos)

#define RIGHTS_USR			(uint8_t)(0b01 << RIGHTS_Pos)
#define RIGHTS_ADMIN		(uint8_t)(0b10 << RIGHTS_Pos)
#define RIGHTS_SYSTEM		(uint8_t)(0b11 << RIGHTS_Pos)

#define HANDSHAKE_HEADER_LEN			0x6
#define SESSION_RECONNECT_HEADER_LEN	0x3

#define MSGTYPE_HANDSHAKEACK		(uint8_t)(0x9 << MsgType_Pos)
#define HANDSHAKEACK_AUTH_SUCC		(uint8_t)(0x1 << 0x3)
#define HANDSHAKEACK_TOPICCREAT_SUCC (uint8_t)(0x1 << 0x2)
#define HANDSHAKEACK_ACC_CREAT_SUCC	(uint8_t)(0x1 << 0x1)
#define HANDSHAKEACK_SECRET_PASSED	(uint8_t)(0x1 << 0x0) 


class Handshaker: public IConnectionHandler {
	std::weak_ptr<IStateContext> context;
	std::unique_ptr<IConnection> connection;
	std::shared_ptr<SessionBuilder> builder;
	std::shared_ptr<IDataAccessor> dbCommunicator;
	std::shared_ptr<SessionRegistry> registry;

protected:
	SessionConfig parseConfig(Packet const& packet) const;
	IConnection::Status sendHandshakeAck(uint8_t ackByte, TopicSet& topicSet) const;
	uint16_t getSessionIdReconnection(Packet const& packet) const;

public:
	Packet serializeReconnectPacket(uint16_t sessionId) const;

	Packet serializeHandshakePacket(bool role, uint8_t qos, bool creat, bool newUsr,
			uint8_t rights, std::string topic, std::string login, std::string password, 
			std::string secret) const;

	Handshaker(std::unique_ptr<IConnection> conn, 
			std::weak_ptr<IStateContext> ctx,
			std::shared_ptr<SessionBuilder> builder,
			std::shared_ptr<SessionRegistry> registry,
			std::shared_ptr<DatabaseCommunicator> dbComm);

	Handshaker(std::unique_ptr<IConnection> conn, 
			std::weak_ptr<IStateContext> ctx);


	int getDescriptor() const override;
	void setConnection(std::unique_ptr<IConnection> conn) override;
	void setContext(std::weak_ptr<IStateContext> ctx) override;
	void makeHandshake(Packet& packet);

	IConnection::Status send(Packet& packet, int flag = 0) const override;
	IConnection::Status recv(Packet& packet, int flag = 0) override;

	IConnection::Status recv(std::string& message, int flag) override;
	IConnection::Status send(std::string const& message, int flag) const override;
};
