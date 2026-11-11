#include "ReconnectState.hpp"


ReconnectState::ReconnectState(std::unique_ptr<IConnection> conn, 
			std::shared_ptr<IStateContext> ctx,
			std::shared_ptr<NodeRegistry> reg, 
			std::shared_ptr<IDataAccessor> stor):
	connection{std::move(conn)}, registry{reg}, storage{stor}, context{ctx} {}

ReconnectState::ReconnectState(std::unique_ptr<IConnection> conn, 
		std::shared_ptr<IStateContext> ctx,
		std::shared_ptr<NodeRegistry> reg, 
		std::shared_ptr<IDataAccessor> stor,
		std::shared_ptr<TimerQueue> timer):
connection{std::move(conn)}, registry{reg}, storage{stor}, context{ctx}, timer{timer} {}



	
IConnection::Status ReconnectState::recv(Packet& packet, int flag) {
	auto status = connection->recv(packet, flag);

	if(status != IConnection::Status::SUCCESS) {
		logError << "ReconnectState::recv(Packet&) failed";
		return(status);
	}

	ReconnectState::ReconnectHeader reconHeader = parseMsg(packet);

	logInfo << std::string{"ReconnectState::recv(): got package "} + 
		std::to_string(packet.header.size()) + " and " + 
		std::to_string(packet.payload.size());

	UserAuth sessionData = registry->findSession(reconHeader.sessionId);

	if(registry->isEmpty(sessionData)) {
		logError << "session's not found";
		return(IConnection::Status::ERROR);
	}

	if(reconHeader.qosLevel == 1) {
		connection = std::make_unique<Qos1>(std::move(connection), timer);
	} else if(reconHeader.qosLevel == 2) {
		//connection = std::make_unique<Qos2>(std::move(connection));
	}

	if(auto strong = context.lock()) {
		strong->changeStateSession(std::move(connection), strong, sessionData);
	}

	return(status);
}

void ReconnectState::trimReconnHeader(Packet& packet) const {
	if(packet.header.size() < RECONNECT_HEADERLEN) return;
	for(uint8_t i = 0; i < RECONNECT_HEADERLEN; ++i) {
		packet.header.pop_back();
	}
}


IConnection::Status ReconnectState::send(Packet& packet, int flag) const {
	return(connection->send(packet, flag));
}


IConnection::Status ReconnectState::recv(std::string& message, int flag) {
	Packet packet;
	auto status = connection->recv(packet, flag);
	packet.payload = message;
	return(status);
}

IConnection::Status ReconnectState::send(std::string const& message, int flag) const {
	Packet packet;
	packet.payload = message;
	auto status = connection->send(packet, flag);
	return(status);
}


void ReconnectState::setContext(std::shared_ptr<IStateContext> ctx) {
	context = ctx;
}


ReconnectState::ReconnectHeader ReconnectState::parseMsg(Packet const& packet) const {
	ReconnectState::ReconnectHeader rh;

	if(packet.header.size() < RECONNECT_HEADERLEN) {
		logError << "ReconnectState::parseMsg() failed: packet size is corrupted";
		return(rh);
	}

	if(packet.header[0] == MSGTYPE_RECONNECT) {

		rh.sessionId = packet.header[1] | (packet.header[2] << 0x8);
		rh.qosLevel = (packet.header[0] >> 0x2) & 0b11;

	} else {
		logError << "ReconnectState::parseMsg(Packet const&) failed: msgtype is unknown";
	}

	return(rh);
}


int ReconnectState::getDescriptor() const {
	return(connection->getDescriptor());
}


void ReconnectState::setConnection(std::unique_ptr<IConnection> conn) {
	connection = std::move(conn);
}

std::unique_ptr<IConnection> ReconnectState::getConnection() {
	return(std::move(connection));
}

