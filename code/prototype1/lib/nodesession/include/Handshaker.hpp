#pragma once
#include "IConnectionHandler.hpp"
#include "NodeRegistry.hpp"
#include "IStateContext.hpp"
#include "Session.hpp"
#include "CacheAccessor.hpp"
#include "IDataAccessor.hpp"
#include "SessionConfig.hpp"
#include "Qos1.hpp"



#ifndef MsgType_Pos
#define MsgType_Pos				0x4
#endif

#define HANDSHAKER_HEADERLEN	0x6
#define RoleType_Pos			0x2
#define QosType_Pos				0x0

// second byte
#define CREATFLAG_Pos			0x5
#define NEWUSRFLAG_Pos			0x4
#define RIGHTS_Pos				0x2

// bit definitions
#define MSGTYPE_HANDSHAKE		(uint8_t)(0x06 << MsgType_Pos)
#define QOSTYPE_0				(uint8_t)(0x0 << QosType_Pos)
#define QOSTYPE_1				(uint8_t)(0x1 << QosType_Pos)
#define QOSTYPE_2				(uint8_t)(0x2 << QosType_Pos)

#define RIGHTS_USR				(uint8_t)(0b01)
#define RIGHTS_ADMIN			(uint8_t)(0b10)
#define RIGHTS_SYSTEM			(uint8_t)(0b11)

#define ROLE_PROD				(uint8_t)(0x1)
#define ROLE_CONS				(uint8_t)(0x2)
#define ROLE_CONSPROD			(uint8_t)(0x3)

// ack byte
#define ACKBYTE_HEADERLEN		0x1
#define AUTHSTATUS_Pos			0x3
#define TOPICCREAT_Pos			0x2
#define USRCREAT_Pos			0x1
#define SECRETPASS_Pos			0x0

#define MSGTYPE_HANDSHAKE_ACK	(uint8_t)(0x9 << MsgType_Pos)
#define AUTH_SUCCESS			(uint8_t)(0x1 << AUTHSTATUS_Pos)
#define USRCREAT_SUCCESS		(uint8_t)(0x1 << USRCREAT_Pos)
#define TOPICCREAT_SUCCESS		(uint8_t)(0x1 << TOPICCREAT_Pos)
#define SECRETPASS_SUCCESS		(uint8_t)(0x1 << SECRETPASS_Pos)


class Handshaker: public IConnectionHandler {
	std::weak_ptr<IStateContext> context;
	std::unique_ptr<IConnection> connection;
	std::shared_ptr<IDataAccessor> dataAccessor;
	std::shared_ptr<NodeRegistry> nodeRegistry;
	
public:
	SessionConfig parseMsg(Packet const& packet) const;

	Handshaker(std::shared_ptr<IStateContext> ctx, 
			std::unique_ptr<IConnection> conn, 
			std::shared_ptr<IDataAccessor> dataAcc, 
			std::shared_ptr<NodeRegistry> nodeReg);

	IConnection::Status recv(Packet& packet, int flag = 0) override;
	IConnection::Status send(Packet& packet, int flag = 0) const override;

	IConnection::Status recv(std::string& message, int flag = 0) override;
	IConnection::Status send(std::string const& message, int flag = 0) const override;

	void setConnection(std::unique_ptr<IConnection> conn) override;
	std::unique_ptr<IConnection> getConnection() override;

	void setContext(std::shared_ptr<IStateContext> ctx) override;
	int getDescriptor() const override;
	bool isInitialized() const override { return(false); }
};
