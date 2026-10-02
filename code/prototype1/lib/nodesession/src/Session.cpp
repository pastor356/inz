#include "Session.hpp"

uint16_t Session::sessionId = 300;

Session::Session(std::unique_ptr<IConnection> conn, std::shared_ptr<IStateContext> ctx,
			std::string const& passwd, std::string const& secrt):
				connection{std::move(conn)}, context{ctx}, 
				password{passwd}, secret{secrt} {
	thisSessionId = ++sessionId;
	state = SessionState::CONNECTED;
}


Session::Session(std::string const& login, std::string const& password,
		std::string const& secret): login{login}, password{password}, secret{secret} {
	thisSessionId = ++sessionId;
	state = SessionState::CONNECTED;
}


IConnection::Status Session::send(std::string const& message, int flag) const {
	Packet packet;
	packet.payload = message;

	auto status = this->send(packet, flag);

	return(status);
}




IConnection::Status Session::recv(Packet& packet, int flag) {
	auto status = connection->recv(packet, flag);
	std::string methodName = "Session::recv(Packet&)";

	if(status != IConnection::Status::SUCCESS) {
		logError << methodName + ": failed";
		return(status);
	}

	SessionHeader sh = parseMsg(packet);

	if (sh.sessionId != thisSessionId) {
		logError << std::string{methodName} + ": sessionId mismatch";
		return IConnection::Status::ERROR;
	}

	return(status);
}


Session::SessionHeader Session::parseMsg(Packet& packet) const {
    const char* methodName = "Session::parseMsg(Packet&)";
	Session::SessionHeader sessionHeader{};

    if (packet.header.size() < SESSIONHEADER_LEN) {
        logError << std::string{methodName} + ": header length doesn't fit";
        return sessionHeader;
    }

    uint8_t msgType = packet.header[0];
    if (msgType != MSGTYPE_SESSIONMSG) {
        logError << std::string{methodName} + ": msg type is unknown";
        return sessionHeader;
    }

    sessionHeader.sessionId =
        static_cast<uint16_t>(packet.header[1]) |
        (static_cast<uint16_t>(packet.header[2]) << 0x8);

    uint64_t timeStampNs = 0;
    for (uint8_t i = 0; i < sizeof(uint64_t); ++i) {
        timeStampNs |= (static_cast<uint64_t>(packet.header[3 + i]) << (i * 0x8));
    }

    sessionHeader.timeStamp = std::chrono::nanoseconds(timeStampNs);

    return sessionHeader;
}

	
	

IConnection::Status Session::recv(std::string& message, int flag) {
	Packet packet;

	auto status = this->recv(packet, flag);
	
	message = packet.payload;

	return(status);
}


IConnection::Status Session::send(Packet& packet, int flag) const {
	IConnection::Status status;

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

void Session::setContext(std::weak_ptr<IStateContext> ctx) {
	context = ctx;
}

uint16_t Session::getId() const {
	return(thisSessionId);
}


