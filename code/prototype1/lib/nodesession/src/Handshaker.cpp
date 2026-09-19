#include "Handshaker.hpp"


Handshaker::Handshaker(std::unique_ptr<IConnection> connection, 
		std::shared_ptr<IStateContext> ctx, std::shared_ptr<IStateFabric> sessionFabric): 
			connection(std::move(conn)), 
			context(std::move(ctx)), 
			fabric(std::move(sessionFabric)) {}


void Handshaker::setContext(std::shared_ptr<IStateContext> ctx) {
	this->context = ctx;
}


IConnection::Status Handshaker::send(Packet& packet, int flag) const {
	// add handshaker header
	return(connection->send(packet, flag));
}


IConnection::Status Handshaker::recv(Packet& packet, int flag) const {
	auto status = connection->recv(packet, flag);
	if(status != IConnection::Status::SUCCESS) {
		logError << "recv() failed: " + strerror(errno);
		return(status);
	}

	SessionConfig sc = parseConfig(packet);

	context->changeState(this->fabric(sc));

	return(status);
}

	
SessionConfig Handshaker::parseConfig(Packet const& packet) const {
	size_t headerSize = packet.header.size();
	SessionConfig sc;
	uint8_t loginLen, passwdLen, secretLen;

	if(headerSize >= SESSIONHEADER_LEN) {
		uint8_t firstByte = packet.header[headerSize - 5];
		uint8_t secondByte = packet.header[headerSize - 4];

		loginLen = packet.header[headerSize - 3];
		passwdLen = packet.header[headerSize - 2];
		secretLen = packet.header[headerSize - 1];
		 
		if((firstByte & (0b1111 << MsgType_Pos)) != MSGTYPE_HANDSHAKE) {
			logError << "unknown MsgType";
			return(sc);
		}
		
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
			logError << "unknown QosLevel";
		}

		sc.creatFlag = secondByte & (0b1 << CREATFLAG_Pos);
		sc.newUsr = secondByte & (0b1 << NEWUSRFLAG_Pos);
		sc.rights = secondByte & (0b11 << RIGTHS_Pos);

		if(sc.rights != RIGHTS_USR && 
			sc.rights != RIGHTS_ADMIN && 
			sc.rights != RIGHTS_SYSTEM) logError << "unknown rights";

	}

	if(payload.size() >= (loginLen + passwdLen + secretLen)) {
		auto cursor = packet.payload.begin();

		sc.login.assign(cursor, cursor + loginLen);
		cursor += loginLen;

		sc.passwd.assign(cursor, cursor + passwdLen);
		cursor += passwdLen;

		sc.secret.assign(cursor, cursor + secretLen);
	} else {
		logError << "size of payload doesn't fit";
	}


	return(sc); 
}

