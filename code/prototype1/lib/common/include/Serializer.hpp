#pragma once
#include <cstdint>
#include "Packet.hpp"
#include "ConsoleLogger.hpp"


namespace Serializer {

Packet serializeHandshake(uint8_t role, uint8_t qos, bool creat, bool newUsr,
			uint8_t rights, std::string topic, std::string login,
			std::string password, std::string secret);

Packet serializeInitial(bool isReconnect = false);
Packet serializeReconnect(uint16_t sessionId);
Packet serializeSession(uint16_t sessionId);
Packet serializeData(std::string const& topic);
};


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


#define MSGTYPE_TOPICDATA		(uint8_t)(0xa << MsgType_Pos)
#define TOPICDATA_HEADERLEN		0x4

#define MSGTYPE_SESSIONMSG	(uint8_t)(0x8 << MsgType_Pos)
#define SESSIONHEADER_LEN	(uint8_t)11


#define MsgType_Pos		0x4
#define PacketId_Pos	0x8
#define MSGTYPE_DATA	(uint8_t)(0x1 << MsgType_Pos)
#define MSGTYPE_PUBACK	(uint8_t)(0x2 << MsgType_Pos)
#define MSGTYPE_PUBREC	(uint8_t)(0x3 << MsgType_Pos)
#define MSGTYPE_PUBREL	(uint8_t)(0x4 << MsgType_Pos)
#define MSGTYPE_PUBCOMP	(uint8_t)(0x5 << MsgType_Pos)

#define QOSHEADER_LEN 3

#define MSGTYPE_RECONNECT	(uint8_t)(0x7 << MsgType_Pos)
#define RECONNECT_HEADERLEN 0x3
