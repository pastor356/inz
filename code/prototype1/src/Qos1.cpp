#include "Qos1.hpp"

IConnection::Status Qos1::send(std::string const& msg, int flag) const {
	Packet packet;
	packet.payload = msg;
	auto status = this->send(packet);
	if(status != IConnection::Status::SUCCESS) return(status);

	return(status);
}



IConnection::Status Qos1::recv(std::string& msg, int flag) const {
	Packet packet;
	auto status = this->recv(packet);
	if(status != IConnection::Status::SUCCESS) return(status);
	msg = packet.payload;

	return(status);
}



IConnection::Status Qos1::recv(Packet& packet, int flag) const {
	auto status = connection->recv(packet);

	if(status == IConnection::Status::ERROR || status == IConnection::Status::HUP) {
		return(status);
	}

	auto pending = parseMsg(packet);

	if(!pending) {
		return(IConnection::Status::ERROR);
	}

	
	if(pending->msgType == MsgType::DATA) {
		status = sendPubAck(pending->packetId);
	} else if(pending->msgType == MsgType::PUBACK && 
			pending->state == QosState::FINISHED) { // this Msg

		auto iter = pendingMessages.find(pending->packetId);

		logInfo << "received PubAck";

		// msg found in map
		if(iter != pendingMessages.end() && iter->second.state == QosState::WAIT_PUBACK) 
			pendingMessages.erase(iter);
	}

	trimQosHeader(packet.header); 

	return(status);
}




std::shared_ptr<IQualityStrategy::PendingMessage> Qos1::parseMsg(Packet const& packet) const {
	auto retMsg = std::make_shared<PendingMessage>();

	if(packet.header.size() >= QOSHEADER_LEN) {
		retMsg->msgType = static_cast<MsgType>(packet.header[0]);
		retMsg->packetId = (packet.header[1] & 0xff) |
			((packet.header[2] << 0x8) & 0xff);
		
		retMsg->state = (retMsg->msgType == MsgType::DATA)?
			QosState::WAIT_PUBACK : QosState::FINISHED;

		retMsg->lastSent = std::chrono::steady_clock::now(); 
		
		retMsg->payload = packet.payload;

		return(retMsg);
	} else {
		logError << "packet.header.size() >= QOSHEADER_LEN";
		return(nullptr);
	}

}




IConnection::Status Qos1::send(Packet& packet, int flag) const {
	// add QOS header
	packet.header.push_back(MSGTYPE_DATA);
	packet.header.push_back(this->currMsgId & 0xff);
	packet.header.push_back((this->currMsgId >> 0x8) & 0xff);

	uint16_t thisMsgId = currMsgId++;

	auto status = connection->send(packet);

	PendingMessage pending{thisMsgId, MsgType::DATA, 
		std::chrono::steady_clock::now(), packet.payload, 
		QosState::WAIT_PUBACK, 0};


	pendingMessages.insert(std::pair<uint16_t, PendingMessage>(thisMsgId, pending));

	return(status);
}




void Qos1::trimQosHeader(std::vector<uint8_t>& header) const {
    if (header.size() < QOSHEADER_LEN) return;

    for (size_t i = 0; i < QOSHEADER_LEN; ++i) {
        header.pop_back();
    }
}



IConnection::Status Qos1::sendPubAck(uint16_t packetId) const {
	Packet ackPacket;
    ackPacket.header.push_back(MSGTYPE_PUBACK);
    ackPacket.header.push_back(packetId & 0xff);
    ackPacket.header.push_back((packetId >> 8) & 0xff);

	logInfo << "sending PubAck";
	return(connection->send(ackPacket));
}



IConnection::Status Qos1::sendPubRec(uint16_t packetId) const {
	auto iter = pendingMessages.find(packetId);

	if(iter != pendingMessages.end() && 
			iter->second.state == QosState::WAIT_PUBREC) {
		std::string tmp;
		tmp.push_back(MSGTYPE_PUBREC);

		return(connection->send(tmp));
	}
	return(IConnection::Status::AGAIN);
}



IConnection::Status Qos1::sendPubRel(uint16_t packetId) const {
	auto iter = pendingMessages.find(packetId);

	if(iter != pendingMessages.end() && 
			iter->second.state == QosState::WAIT_PUBREL) {
		std::string tmp;
		tmp.push_back(MSGTYPE_PUBREL);

		return(connection->send(tmp));
	}
	return(IConnection::Status::AGAIN);
}



IConnection::Status Qos1::sendPubComp(uint16_t packetId) const {
	auto iter = pendingMessages.find(packetId);

	if(iter != pendingMessages.end() && 
			iter->second.state == QosState::WAIT_PUBCOMP) {
		std::string tmp;
		tmp.push_back(MSGTYPE_PUBCOMP);

		return(connection->send(tmp));
	}
	return(IConnection::Status::AGAIN);
}
