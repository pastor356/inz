#include "Handshaker.hpp"

Handshaker::Handshaker(std::shared_ptr<IStateContext> ctx, 
		std::unique_ptr<IConnection> conn, 
		std::shared_ptr<IDataAccessor> dataAcc, 
		std::shared_ptr<NodeRegistry> nodeReg):
	context{ctx}, connection{std::move(conn)}, 
	dataAccessor{dataAcc}, nodeRegistry{nodeReg} {}


IConnection::Status Handshaker::recv(Packet& packet, int flag) {
	auto status = connection->recv(packet, flag);
	uint8_t ackByte = 0;
	Packet ackPacket;

	SessionConfig sc = parseMsg(packet);
	IDataAccessor::AccessorStatus authStatus;

	if(sc.newUsr) {
		authStatus = dataAccessor->createUser(sc.authData);

		if(authStatus != IDataAccessor::AccessorStatus::SUCCESS) {
			logError << "user creation failed";
		}

		if(authStatus == IDataAccessor::AccessorStatus::SUCCESS) {
			ackByte |= USRCREAT_SUCCESS;
			ackByte |= SECRETPASS_SUCCESS;
		}

	} else {
		authStatus = dataAccessor->authorize(sc.authData);

		if(authStatus != IDataAccessor::AccessorStatus::SUCCESS) {
			logError << "user authorization failed";
		}

		if(authStatus == IDataAccessor::AccessorStatus::SUCCESS) {
			ackByte |= AUTH_SUCCESS;
			ackByte |= SECRETPASS_SUCCESS;
		}
	}

	if(sc.creatFlag) {
		dataAccessor->addTopic(sc.topic);
		ackByte |= TOPICCREAT_SUCCESS;
	}


	ackByte |= MSGTYPE_HANDSHAKE_ACK;
	ackPacket.header.push_back(ackByte);
	ackPacket.payload.clear(); 

	auto sendStatus = connection->send(ackPacket, 0);
	if(sendStatus != IConnection::Status::SUCCESS) {
		logError << "sending ack failure";
		return(sendStatus);
	}

	if(sc.qosLevel == 1) {
		connection = std::make_unique<Qos1>(std::move(connection));
	} else if(sc.qosLevel == 2) {
		//connection == std::make_unique<Qos2>(std::move(connection));
	}

	if(auto strong = context.lock()) {
		strong->changeStateSession(std::move(connection), strong, sc.authData);
	}


	return(sendStatus);
}



SessionConfig Handshaker::parseMsg(Packet const& packet) const {
	const char* methodName = "Handshaker::parseMsg(Packet const&)";
	SessionConfig sc;
	size_t loginLen, passwdLen, secretLen, topicLen;
	size_t headerSize = packet.header.size();

	if(packet.header.size() < HANDSHAKER_HEADERLEN) {
		logError << "Handshaker::parseMsg() failed: header size doesn't fit";
		return(sc);
	}

	uint8_t firstByte = packet.header[headerSize - 6];
	uint8_t msgType = firstByte & (0b1111 << MsgType_Pos);



	if(msgType == MSGTYPE_HANDSHAKE) {
		if(headerSize != HANDSHAKER_HEADERLEN) {
			logError << std::string{methodName} + ": header size doesn't fit";
			return(sc);
		}

		uint8_t secondByte = packet.header[headerSize - 5];

		loginLen = packet.header[headerSize - 4];
		passwdLen = packet.header[headerSize - 3];
		secretLen = packet.header[headerSize - 2];
		topicLen = packet.header[headerSize - 1];

		if(firstByte & (0b1 << RoleType_Pos)) {
			sc.isProducer = true;
		}

		uint8_t qosLev = firstByte & (0b11 << QosType_Pos);

		if(qosLev == QOSTYPE_1) {
			sc.qosLevel = 1;
		} else if(qosLev == QOSTYPE_0) {
			sc.qosLevel = 0;
		} else if(qosLev == QOSTYPE_2) {
			sc.qosLevel = 2;
		} else {
			logError << std::string{methodName} + ": unknown QosLevel";
		}

		sc.creatFlag = (secondByte >> CREATFLAG_Pos) & 0b1;
		sc.newUsr    = (secondByte >> NEWUSRFLAG_Pos) & 0b1;
		sc.authData.rights    = (secondByte >> RIGHTS_Pos) & 0b11;
		sc.rights    = (secondByte >> RIGHTS_Pos) & 0b11;

		if(sc.rights != RIGHTS_USR &&
			sc.rights != RIGHTS_ADMIN &&
			sc.rights != RIGHTS_SYSTEM)
			logError << std::string{methodName} + ": unknown rights";

		if(packet.payload.size() >= (loginLen + passwdLen + secretLen)) {
			auto cursor = packet.payload.begin();

			sc.topic.assign(cursor, cursor + topicLen);
			cursor += topicLen;

			sc.authData.login.assign(cursor, cursor + loginLen);
			cursor += loginLen;

			sc.authData.password.assign(cursor, cursor + passwdLen);
			cursor += passwdLen;

			sc.authData.secret.assign(cursor, cursor + secretLen);

		}
	} else {
		logError << std::string{methodName} + ": unknown msg type in header";
		return(sc);
	}

	return(sc);
}


IConnection::Status Handshaker::send(Packet& packet, int flag) const {
	return(connection->send(packet, flag));
}


int Handshaker::getDescriptor() const {
	return(connection->getDescriptor());
}

void Handshaker::setContext(std::shared_ptr<IStateContext> ctx) {
	context = ctx;
}

void Handshaker::setConnection(std::unique_ptr<IConnection> conn) {
	connection = std::move(conn);
} 



IConnection::Status Handshaker::recv(std::string& message, int flag) {
	Packet packet;
	auto status = this->recv(packet, flag);
	message = packet.payload;

	return(status);
}


IConnection::Status Handshaker::send(std::string const& message, int flag) const {
	Packet packet;
	packet.payload = message;

	return(this->send(packet, flag));
}


std::unique_ptr<IConnection> Handshaker::getConnection() {
	return(std::move(connection));
}
