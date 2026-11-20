#include "Session.hpp"

uint16_t Session::sessionId = 300;

Session::Session(std::unique_ptr<IConnection> conn, std::shared_ptr<IStateContext> ctx, 
		UserAuth const& userAuth):
	connection{std::move(conn)}, context{ctx}, authData{userAuth} {
	
	thisSessionId = sessionId++;
	
	Packet packet;
	packet.payload.resize(sizeof(thisSessionId));
	memcpy(packet.payload.data(), &thisSessionId, sizeof(thisSessionId)); // UNSAFE

	connection->send(packet);
}

Session::Session(std::unique_ptr<IConnection> conn, std::shared_ptr<IStateContext> ctx,
			UserAuth const& userAuth, std::shared_ptr<TimerQueue> timer):
	connection{std::move(conn)}, context{ctx}, authData{userAuth}, timer{timer} {

	thisSessionId = sessionId++;
	
	Packet packet;
	packet.payload.resize(sizeof(thisSessionId));
	memcpy(packet.payload.data(), &thisSessionId, sizeof(thisSessionId)); // UNSAFE

	connection->send(packet);
}



IConnection::Status Session::send(std::string const& message, int flag) const {
	Packet packet;
	packet.payload = message;
	const char* methodName = "Session::send(std::string const&, int)"; 

	auto status = this->send(packet, flag);
	if(status == IConnection::Status::ERROR || 
			status == IConnection::Status::HUP) {
		if(auto strong = context.lock()) {
			strong->changeStateTerminated(strong, authData);
		} else  
			logError << std::string{methodName} + " failed: context ptr is nullptr";
	}

	return(status);
}


IConnection::Status Session::recv(Packet& packet, int flag) {
	auto status = connection->recv(packet, flag);
	std::string methodName = "Session::recv(Packet&, int)";

	if(status == IConnection::Status::ERROR || 
			status == IConnection::Status::HUP) {
		if(auto strong = context.lock()) {
			strong->changeStateTerminated(strong, authData);
		} else 
			logError << std::string{methodName}+ " failed: context ptr is nullptr";
	}

	if(status != IConnection::Status::SUCCESS) {
		logError << methodName + ": failed";
		return(status);
	}

	if(packet.header.size() < SESSIONHEADER_LEN) return(status);

	SessionHeader sh = parseMsg(packet);

	if (sh.sessionId != thisSessionId) {
		logError << std::string{methodName} + ": sessionId mismatch: "
			+ std::to_string(sh.sessionId) + " vs. thisSessionId = " 
			+ std::to_string(thisSessionId);
		return IConnection::Status::ERROR;
	}

	trimSessionHeader(packet);

	return(status);
}

void Session::trimSessionHeader(Packet& packet) const {
	if(packet.header.size() >= SESSIONHEADER_LEN) {

		for(unsigned int i = 0; i < SESSIONHEADER_LEN; ++i) 
			packet.header.pop_back();

	} else 
		logError << "Session::trimSessionHeader(Packet&) failed: header is corrupted";
}



Session::SessionHeader Session::parseMsg(Packet& packet) const {
    const char* methodName = "Session::parseMsg(Packet&)";
	Session::SessionHeader sessionHeader{};

    if (packet.header.size() < SESSIONHEADER_LEN) {
        logError << std::string{methodName} + ": header length doesn't fit";
        return sessionHeader;
    }

	uint8_t sessionHeaderOffset = packet.header.size() - SESSIONHEADER_LEN;

    uint8_t msgType = packet.header[sessionHeaderOffset];
    if (msgType != MSGTYPE_SESSIONMSG) {
        logError << std::string{methodName} + ": msg type is unknown";
        return sessionHeader;
    }

    sessionHeader.sessionId =
        static_cast<uint16_t>(packet.header[sessionHeaderOffset + 1]) |
        (static_cast<uint16_t>(packet.header[sessionHeaderOffset + 2]) << 0x8);

    uint64_t timeStampNs = 0;
    for (uint8_t i = 0; i < sizeof(uint64_t); ++i) {
        timeStampNs |= (static_cast<uint64_t>(
					packet.header[(sessionHeaderOffset + 3) + i]) << (i * 0x8));
    }

    sessionHeader.timeStamp = std::chrono::nanoseconds(timeStampNs);

    return sessionHeader;
}

	
	

IConnection::Status Session::recv(std::string& message, int flag) {
	Packet packet;
	const char* methodName = "Session::recv(std::string const&, int)";

	auto status = this->recv(packet, flag);
	
	if(status == IConnection::Status::ERROR || 
			status == IConnection::Status::HUP) {
		if(auto strong = context.lock()) {
			strong->changeStateTerminated(strong, authData);
		} else 
			logError <<  std::string{methodName} + " failed: context ptr is nullptr";

	}

	message = packet.payload;

	return(status);
}


IConnection::Status Session::send(Packet& packet, int flag) const {
	IConnection::Status status;
	const char* methodName = "Session::send(Packet&, int)";
	
	auto time_stamp_ns = std::chrono::duration_cast<std::chrono::nanoseconds>
		(std::chrono::system_clock::now().time_since_epoch()).count();

	size_t time_stamp_size = sizeof(time_stamp_ns);

	packet.header.push_back(MSGTYPE_SESSIONMSG);
	packet.header.push_back(thisSessionId & 0xff);
	packet.header.push_back((thisSessionId >> 0x8) & 0xff);

	for(uint8_t i = 0; i < time_stamp_size ; ++i) { // push back timestamp into header
		packet.header.push_back((time_stamp_ns >> (i*0x8)) & 0xff);
	}
	
	if((status = connection->send(packet, flag)) != IConnection::Status::SUCCESS)
		logError << "Session::send() failed";

	if(status == IConnection::Status::ERROR || 
			status == IConnection::Status::HUP) {
		if(auto strong = context.lock()) {
			strong->changeStateTerminated(strong, authData);
		} else 
			logError << std::string{methodName} + "  failed: context ptr is nullptr";
	}

	return(status);
}


void Session::setConnection(std::unique_ptr<IConnection> conn) {
	this->connection = std::move(conn);
}

int Session::getDescriptor() const {
	return(connection->getDescriptor());
}

void Session::handleTimeout() {
	state = SessionState::DISCONNECTED;
}

uint8_t Session::getQos() const {
	return(this->qosLevel);
}

void Session::setQos(uint8_t level) {
	qosLevel = level;
}

void Session::setContext(std::shared_ptr<IStateContext> ctx) {
	context = ctx;
}

uint16_t Session::getId() const {
	return(thisSessionId);
}

std::unique_ptr<IConnection> Session::getConnection() {
	return(std::move(connection));
}
