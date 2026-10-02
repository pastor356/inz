#include "Handshaker.hpp"


	
Handshaker::Handshaker(std::unique_ptr<IConnection> conn, 
			std::weak_ptr<IStateContext> ctx,
			std::shared_ptr<SessionBuilder> builder,
			std::shared_ptr<SessionRegistry> registry,
			std::shared_ptr<DatabaseCommunicator> dbComm): 
	context{ctx}, connection{std::move(conn)}, builder{builder}, 
	dbCommunicator{dbComm}, registry{registry} {}

Handshaker::Handshaker(std::unique_ptr<IConnection> conn, 
			std::weak_ptr<IStateContext> ctx): connection{std::move(conn)}, context{ctx} {}



IConnection::Status Handshaker::send(Packet& packet, int flag) const {

	// add handshaker header
	return(connection->send(packet, flag));
}



IConnection::Status Handshaker::recv(Packet& packet, int flag) {
	auto status = connection->recv(packet, flag);
	const char* methodName = "Handshaker::recv()";

	if(status != IConnection::Status::SUCCESS) {
		logError << std::string{methodName} + " failed: " + strerror(errno);
		return(status);
	}

	makeHandshake(packet);

	return(status);
}



void Handshaker::makeHandshake(Packet& packet) {
	const char* methodName = "Handshaker::makeHandshake()";
	uint16_t sessionId;
	SessionConfig sc;

	if(packet.header[0] == MSGTYPE_HANDSHAKE) {
		sc = parseConfig(packet);
		auto session = std::make_shared<Session>(sc.login, sc.passwd, sc.secret);

		auto monitor = std::make_unique<LiveMonitor>(std::move(connection));
		std::weak_ptr<Session> weakSession = session;

		monitor->setCallback([weakSession]() {
			if (auto activeSession = weakSession.lock()) {
				activeSession->handleTimeout(); 
			}
		});

		connection = std::move(monitor);


		if(sc.qosLevel == 1) {
			connection = std::make_unique<Qos1>(std::move(connection));
			session->setQos(1);
		} else if(sc.qosLevel == 2) {
			//connection = std::make_unique<Qos2>(std::move(connection));
			session->setQos(2);
		}

		// DB QUERY AND AUTHORIZATION
		TopicSet topicSet; 
		DatabaseCommunicator::DbStatus status;

		if(sc.newUsr) {
			status = dbCommunicator->createUser(sc.login, 
					sc.passwd, sc.secret, sc.rights);
		} else {
			status = dbCommunicator->authorize(sc.login, sc.passwd, sc.secret, sc.rights);
		}
		
		if(status == DatabaseCommunicator::DbStatus::SUCCESS) {
			status = dbCommunicator->getTopics(topicSet);
		}

		session->setConnection(std::move(connection));

		registry->addSession(session);

		if (auto ctx = context.lock()) {
			ctx->changeState(session);
		}

	} else if(packet.header[0] == MSGTYPE_RECONNECT) {
		sessionId = getSessionIdReconnection(packet);
	
		auto session = registry->findSession(sessionId);

		if(!session) {
			logError << std::string{methodName} + ": session not found";
			return;
		}

		auto monitor = std::make_unique<LiveMonitor>(std::move(connection));

        std::weak_ptr<Session> weakSession = session;
        monitor->setCallback([weakSession]() {
            if (auto activeSession = weakSession.lock()) {
                activeSession->handleTimeout();
            }
        });

		connection = std::move(monitor);

        if (session->getQos() == 1) { 
            connection = std::make_unique<Qos1>(std::move(connection));
        }

		session->setConnection(std::move(this->connection));

		(context.lock())->changeState(session);
	}
}




SessionConfig Handshaker::parseConfig(Packet const& packet) const {
	SessionConfig sc;
	uint8_t loginLen, passwdLen, secretLen, topicLen;
	size_t headerSize = packet.header.size();

	uint8_t firstByte = packet.header[headerSize - 6];
	uint8_t msgType = firstByte & (0b1111 << MsgType_Pos);

	const char* methodName = "Handshaker::parseConfig()";

		 
	if(msgType == MSGTYPE_HANDSHAKE) {
		if(headerSize != HANDSHAKE_HEADER_LEN) {
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

		sc.creatFlag = secondByte & (0b1 << CREATFLAG_Pos);
		sc.newUsr = secondByte & (0b1 << NEWUSRFLAG_Pos);
		sc.rights = secondByte & (0b11 << RIGHTS_Pos);

		if(sc.rights != RIGHTS_USR && 
			sc.rights != RIGHTS_ADMIN && 
			sc.rights != RIGHTS_SYSTEM) 
			logError << std::string{methodName} + ": unknown rights";

		if(packet.payload.size() >= (loginLen + passwdLen + secretLen)) {
			auto cursor = packet.payload.begin();

			sc.topic.assign(cursor, cursor + topicLen);
			cursor += topicLen;

			sc.login.assign(cursor, cursor + loginLen);
			cursor += loginLen;

			sc.passwd.assign(cursor, cursor + passwdLen);
			cursor += passwdLen;

			sc.secret.assign(cursor, cursor + secretLen);

			// send ack
			auto status = sendHandshakeAck();

			if(status != IConnection::Status::SUCCESS) {
				logError << std::string{methodName} + ": ack sending failed";
			}

		}

	} else {
		logError << std::string{methodName} + ": unknown msg type in header";
		return(sc);
	}
		

	return(sc); 
}



Packet Handshaker::serializeHandshakePacket(bool role, uint8_t qos, bool creat, bool newUsr,
				uint8_t rights, std::string topic, std::string login,
				std::string password, std::string secret) const {
    Packet packet;

    uint8_t firstByte = MSGTYPE_HANDSHAKE;

    if(role) {
        firstByte |= (0b11 << RoleType_Pos);
    }

    firstByte |= ((qos & 0b11) << QosType_Pos);

    uint8_t secondByte = 0;

    if(creat) {
        secondByte |= (0b1 << CREATFLAG_Pos);
    }

    if(newUsr) {
        secondByte |= (0b1 << NEWUSRFLAG_Pos);
    }

    secondByte |= ((rights & 0b11) << RIGHTS_Pos);

    uint8_t loginLen  = static_cast<uint8_t>(login.size());
    uint8_t passwdLen = static_cast<uint8_t>(password.size());
    uint8_t secretLen = static_cast<uint8_t>(secret.size());
    uint8_t topicLen  = static_cast<uint8_t>(topic.size());

    packet.header.resize(HANDSHAKE_HEADER_LEN);
    packet.header[0] = firstByte;   // headerSize - 6
    packet.header[1] = secondByte;  // headerSize - 5
    packet.header[2] = loginLen;    // headerSize - 4
    packet.header[3] = passwdLen;   // headerSize - 3
    packet.header[4] = secretLen;   // headerSize - 2
    packet.header[5] = topicLen;    // headerSize - 1

    packet.payload.reserve(topicLen + loginLen + passwdLen + secretLen);
    packet.payload.insert(packet.payload.end(), topic.begin(), topic.end());
    packet.payload.insert(packet.payload.end(), login.begin(), login.end());
    packet.payload.insert(packet.payload.end(), password.begin(), password.end());
    packet.payload.insert(packet.payload.end(), secret.begin(), secret.end());

    return(packet);
}




Packet Handshaker::serializeReconnectPacket(uint16_t sessionId) const {
	Packet packet;
	packet.header.resize(3);
	packet.header.push_back(MSGTYPE_RECONNECT);
	
	packet.header.push_back(sessionId & 0xff); 
	packet.header.push_back((sessionId >> 0x8) & 0xff);

	return(packet);
}



uint16_t Handshaker::getSessionIdReconnection(Packet const& packet) const {
	const char* methodName = "Handshaker::getSessionIdReconnection()";
	
	size_t headerSize = packet.header.size();

	if(headerSize != SESSION_RECONNECT_HEADER_LEN) {
		logError << std::string{methodName} + ": header size doesn't fit";
		return(0);
	}

	uint8_t msgType = packet.header[0];
	uint16_t sessionId = 0;

	if(msgType == MSGTYPE_RECONNECT) {
		sessionId = packet.header[1] | (packet.header[1] << 0x8);
	} 
	else logError << std::string{methodName} + ": msg type doesn't fit";

	return(sessionId);
}


void Handshaker::setConnection(std::unique_ptr<IConnection> conn) {
	this->connection = std::move(conn);
}


int Handshaker::getDescriptor() const {
	return(connection->getDescriptor());
}

void Handshaker::setContext(std::weak_ptr<IStateContext> ctx) {
	context = ctx;
}

IConnection::Status Handshaker::send(std::string const& message, int flag) const {
	Packet packet;
	packet.payload = message; 

    return(this->send(packet, flag));
}


IConnection::Status Handshaker::recv(std::string& message, int flag) {
	Packet packet;

    auto status = this->recv(packet, flag);
	message = packet.payload;
	
	return(status);
}


IConnection::Status Handshaker::sendHandshakeAck(uint8_t ackByte) const {
	Packet packet;

	packet.header.push_back(ackByte);
	packet.payload.clear();

	return(this->send(packet, 0));
}









