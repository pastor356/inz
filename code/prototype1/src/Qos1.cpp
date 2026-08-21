#include "Qos1.hpp"

IConnection::Status Qos1::send(std::string const& msg, int flag) const {

	std::string message{QOS1_HEADER};
	message += msg;

	auto status = connection->send(message, flag);
	if(IConnection::Status::SUCCESS != status) return(status);

	PendingMessage pendingMsg{++currMsgId, MsgType::REG,
		std::chrono::steady_clock::now(), message, QosState::WAIT_PUBACK, 0};

	pendingMessages.insert(std::pair<uint16_t, PendingMessage>(currMsgId, pendingMsg));

	return(IConnection::Status::SUCCESS);
}


IConnection::Status Qos1::recv(std::string& msg, int flag) const {

	auto status = connection->recv(msg, flag);

	PendingMessage pending = parseMsg(msg);

	if(pending.msgType == MsgType::PUBACK &&
			pending.state == QosState::WAIT_PUBACK) {

		auto iter = pendingMessages.find(pending.packetId);

		// if we're waiting for puback -- erase record, 'cause we just got puback
		if(iter != pendingMessages.end() && iter->second.state == QosState::WAIT_PUBACK)
			pendingMessages.erase(iter);

	} else if(pending.msgType == MsgType::REG) { 
		
		status = sendPubAck(pending.packetId);

	}

	return(status);
}


