#pragma once
#include "IConnectionHandler.hpp"
#include "NodeRegistry.hpp"
#include "IStateContext.hpp"
#include "Session.hpp"
#include "CacheAcceptor.hpp"




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
#define QOSTYPE_0			(uint8_t)(0x0 << QosType_Pos)
#define QOSTYPE_1			(uint8_t)(0x1 << QosType_Pos)
#define QOSTYPE_2			(uint8_t)(0x2 << QosType_Pos)

#define RIGHTS_USR			(uint8_t)(0b01 << RIGHTS_Pos)
#define RIGHTS_ADMIN		(uint8_t)(0b10 << RIGHTS_Pos)
#define RIGHTS_SYSTEM		(uint8_t)(0b11 << RIGHTS_Pos)



class Handshaker: public IConnectionHandler {
	std::weak_ptr<IStateContext> context;
	std::unique_ptr<IConnection> connection;
	std::shared_ptr<IDataAccessor> dataAccessor;
	std::shared_ptr<NodeRegistry> nodeRegistry;
	
public:
	Handshaker(std::shared_ptr<IStateContext> ctx, 
			std::unique_ptr<IConnection> conn, 
			std::shared_ptr<IDataAccessor> dataAcc, 
			std::shared_ptr<NodeRegistry> nodeReg);

	IConnection::Status recv(Packet& packet, int flag = 0) override;
	IConnection::Status send(Packet& packet, int flag = 0) const override;

	void setConnection(std::unique_ptr<IConnection> conn) override;
	void setContext(std::shared_ptr<IStateContext> ctx) override;
	int getDescriptor() const override;
};



