#pragma once
#include "IConnection.hpp"
#include <unordered_map>

class IQualityStrategy: public ConnectionDecorator {
protected:
	enum class QosState {
		WAIT_PUBACK,
		WAIT_PUBREC,
		WAIT_PUBREL,
		WAIT_PUBCOMP
	};

	enum class MsgType {
		REG,
		PUBACK,
		PUBREC,
		PUBREL,
		PUBCOMP
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
	mutable std::unordered_map<uint16_t, PendingMessage> pendindMessages;
	mutable uint16_t currMsgId = 0;

	IConnection::Status sendPubAck(uint16_t packetId);

	IConnection::Status sendPubRec(uint16_t packetId);
	IConnection::Status sendPubRel(uint16_t packetId);
	IConnection::Status sendPubComp(uint16_t packetId);


public:

	IConnection::Status send(std::string const& msg, int flag = 0) const override;
	IConnection::Status recv(std::string& msg, int flag = 0) const override;
};
