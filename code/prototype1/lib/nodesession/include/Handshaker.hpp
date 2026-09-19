#pragma once
#include "IConnectionHandler.hpp"
#include "IStateFabric.hpp"
#include "Session.hpp"

#ifndef MsgType_Pos
#define MsgType_Pos			0x4
#endif
#define RoleType_Pos		0x2
#define QosType_Pos			0x1

// second byte
#define CREATFLAG_Pos		0x5
#define NEWUSRFLAG_Pos		0x4
#define RIGHTS_Pos			0x2

#define MSGTYPE_HANDSHAKE	(uint8_t)(0x06 << MsgType_Pos)
#define QOSTYPE_0			(uint8_t)(0x0 << QosType_Pos)
#define QOSTYPE_1			(uint8_t)(0x1 << QosType_Pos)
#define QOSTYPE_2			(uint8_t)(0x2 << QosType_Pos)

#define RIGHTS_USR			(uint8_t)(0b01 << RIGHTS_Pos)
#define RIGHTS_ADMIN		(uint8_t)(0b10 << RIGHTS_Pos)
#define RIGHTS_SYSTEM		(uint8_t)(0b11 << RIGHTS_Pos)


#define SESSIONHEADER_LEN	0x5

struct SessionConfig {
	uint8_t qosLevel; // 0, 1 or 2
	bool isProducer;
	bool creatFlag;
	bool newUsr;
	std::string login;
	std::string passwd;
	std::string secret;
	uint8_t rights; // 1 -- user, 2 -- admin, 3 -- system
};



class Handshaker: public IConnectionHandler {
	std::shared_ptr<IStateContext> context;
	std::unique_ptr<IConnection> connection;
	std::shared_ptr<IStateFabric> fabric;
	std::unique_ptr<DatabaseCommunicator> dbCommunicator;

	SessionConfig parseConfig(Packet const& packet) const;

public:
	Handshaker(std::unique_ptr<IConnection> conn, 
			std::shared_ptr<IStateContext> ctx,
			std::shared_ptr<IStateFabric> fabric);

	void setContext(std::shared_ptr<IStateContext> ctx) override;
	IConnection::Status send(Packet& packet, int flag = 0) const override;
	IConnection::Status recv(Packet& packet, int flag = 0) const override;
};
