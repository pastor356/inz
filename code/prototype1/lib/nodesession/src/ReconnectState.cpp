#include "ReconnectState.hpp"


ReconnectState::ReconnectState(std::unique_ptr<IConnection> conn, 
			std::shared_ptr<IStateContext> ctx,
			std::shared_ptr<NodeRegistry> reg, 
			std::shared_ptr<IDataAccessor> stor):
	connection{std::move(conn)}, registry{reg}, storage{stor}, context{ctx} {}



	
IConnection::Status ReconnectState::recv(Packet& packet, int flag) {
	auto status = connection->recv(packet, flag);

	if(status != IConnection::Status::SUCCESS) {
		logError << "ReconnectState::recv(Packet&) failed";
		return(status);
	}

	uint16_t sessionId = parseMsg(packet);

	UserAuth sessionData = registry->findSession(sessionId);

	if(registry->isEmpty(sessionData)) {
		logError << "session's not found";
		return(IConnection::Status::ERROR);
	}

	if(auto strong = context.lock()) {
		strong->changeStateSession(std::move(connection), strong, sessionData);
	}

	return(status);
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


uint16_t ReconnectState::parseMsg(Packet const& packet) const {
	if(packet.header.empty()) {
		logError << "ReconnectState::parseMsg() failed: packet is empty";
		return(0);
	}

	uint16_t sessionId = 0;

	if(packet.header[0] == MSGTYPE_RECONNECT) {
		sessionId = packet.header[1] | (packet.header[2] >> 0x8);

	} else {
		logError << "ReconnectState::parseMsg(Packet const&) failed: msgtype is unknown";
	}

	return(sessionId);
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

