#include "InitialState.hpp"

InitialState::InitialState(std::unique_ptr<IConnection> conn, 
		std::shared_ptr<IStateContext> ctx): connection{std::move(conn)}, context{ctx} {}

InitialState::InitialState(std::unique_ptr<IConnection> conn): connection{std::move(conn)} {}

IConnection::Status InitialState::recv(Packet& packet, int flag) {
	const char* methodName = "InitialState::recv(Packet&)";

	auto status = connection->recv(packet, 0);

	if(status != IConnection::Status::SUCCESS) {
		logError << std::string{methodName} + ": recv failed";
		return(status);
	}

	if(isHandshake(packet)) {
		auto strong = context.lock();
		if(strong) {
			strong->changeStateHandshake(std::move(connection), strong);
		}

	} else if(isReconnect(packet)) { 
		if(auto strong = context.lock()) {
			strong->changeStateReconnect(std::move(connection), strong);
		}
	}

	return(status);
}

IConnection::Status InitialState::send(Packet& packet, int flag) const {
	return(connection->send(packet, flag));
}


int InitialState::getDescriptor() const {
	return(connection->getDescriptor());
}

void InitialState::setConnection(std::unique_ptr<IConnection> conn) {
	connection = std::move(conn);
} 

void InitialState::setContext(std::shared_ptr<IStateContext> ctx) {
	context = ctx;
}


bool InitialState::isHandshake(Packet const& packet) const {
	if(packet.header.empty()) return(false);

	if((packet.header[0] & MSGTYPE_HANDSHAKE) == MSGTYPE_HANDSHAKE) {
		return(true);
	}

	return(false);
}

bool InitialState::isReconnect(Packet const& packet) const {
	if(packet.header.empty()) return(false);
	if((packet.header[0] & MSGTYPE_RECONNECT) == MSGTYPE_RECONNECT) {
		return(true);
	}

	return(false);
}


IConnection::Status InitialState::send(std::string const& message, int flag) const {
	Packet packet;
	packet.payload = message;
	return(this->send(packet, flag));
}


IConnection::Status InitialState::recv(std::string& message, int flag) {
	Packet packet;
	auto status = this->recv(packet, flag);
	message = packet.payload;

	return(status);
}


std::unique_ptr<IConnection> InitialState::getConnection() {
	return(std::move(connection));
}


