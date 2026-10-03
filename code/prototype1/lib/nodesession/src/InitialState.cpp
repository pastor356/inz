#include "InitialState.hpp"

InitialState::InitialState(std::unique_ptr<IConnection> conn, 
		std::shared_ptr<IStateContext> ctx): connection{std::move(conn)}, context{ctx} {}


IConnection::Status InitialState::recv(Packet& packet, int flag = 0) const  {
	Packet packet;
	const char* methodName = "InitialState::recv(Packet&)"

	auto status = connection->recv(packet, 0);

	if(status != IConnection::Status::SUCCESS) {
		logError << std::string{methodName} + ": recv failed";
		return(status);
	}

	if(isHandshake(packet)) {
		if((auto strong = context.lock())) {
			strong->changeStateHandshake(std::move(connection), context);
		}

	} else if(isReconnect(packet)) { 
		if((auto strong = context.lock())) {
			strong->changeStateReconnect(std::move(connection), context);
		}
	}

	return(status);
}

IConnection::Status InitialState::send(Packet& packet, int flag = 0) const {
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
