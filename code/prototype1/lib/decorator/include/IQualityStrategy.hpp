#pragma once
#include "IConnection.hpp"
#include "ConnectionDecorator.hpp"
#include "ConsoleLogger.hpp"
#include <unordered_map>
#include <chrono>
#include <vector>

#define MsgType_Pos		0x4
#define PacketId_Pos	0x8
#define MSGTYPE_DATA	(uint8_t)(0x1 << MsgType_Pos)
#define MSGTYPE_PUBACK	(uint8_t)(0x2 << MsgType_Pos)
#define MSGTYPE_PUBREC	(uint8_t)(0x3 << MsgType_Pos)
#define MSGTYPE_PUBREL	(uint8_t)(0x4 << MsgType_Pos)
#define MSGTYPE_PUBCOMP	(uint8_t)(0x5 << MsgType_Pos)

#define QOSHEADER_LEN 3



class IQualityStrategy: public ConnectionDecorator {
protected:
	enum class QosState: uint8_t {
		WAIT_PUBACK,
		WAIT_PUBREC,
		WAIT_PUBREL,
		WAIT_PUBCOMP,
		FINISHED
	};


	enum class MsgType: uint8_t { 
		DATA = MSGTYPE_DATA,
		PUBACK = MSGTYPE_PUBACK,
		PUBREC = MSGTYPE_PUBREC,
		PUBREL = MSGTYPE_PUBREL,
		PUBCOMP = MSGTYPE_PUBCOMP
	};

	struct PendingMessage {
		uint16_t packetId;
		MsgType msgType;
		std::chrono::steady_clock::time_point lastSent;
		std::string payload; // for retry
		QosState state;
		int retries{0};
	};


	// packetId to PendingMessage
	mutable std::unordered_map<uint16_t, PendingMessage> pendingMessages;
	mutable uint16_t currMsgId = 33;

	virtual IConnection::Status sendPubAck(uint16_t packetId) const = 0;
	virtual IConnection::Status sendPubRec(uint16_t packetId) const = 0;
	virtual IConnection::Status sendPubRel(uint16_t packetId) const = 0;
	virtual IConnection::Status sendPubComp(uint16_t packetId) const = 0;

	virtual	std::shared_ptr<PendingMessage> parseMsg(Packet const& packet) const = 0; 
	virtual void trimQosHeader(std::vector<uint8_t>& header) const = 0;

public:
	using ConnectionDecorator::ConnectionDecorator;
	virtual ~IQualityStrategy() = default;

	virtual IConnection::Status send(Packet& packet, int flag = 0) const = 0;
	virtual IConnection::Status recv(Packet& packet, int flag = 0) const = 0;
};
