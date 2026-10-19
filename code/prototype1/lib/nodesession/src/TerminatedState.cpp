#include "TerminatedState.hpp"


TerminatedState::TerminatedState(std::shared_ptr<IStateContext> ctx, 
		UserAuth const& authUser): context{ctx}, auth{authUser} {}


void TerminatedState::setConnection(std::unique_ptr<IConnection> conn) {
	if(!buffer.empty()) {
		for(size_t i = 0; i < buffer.size(); ++i) {
			conn->send(buffer[i]);
		}
	}

	if(auto strong = context.lock()) {
		strong->changeStateReconnect(std::move(conn), strong);
	} else 
		logError << "TerminatedState::setConnection() failed: context ptr is nullptr";
}


std::unique_ptr<IConnection> TerminatedState::getConnection() {
	return(std::unique_ptr<IConnection>());
}

void TerminatedState::setContext(std::shared_ptr<IStateContext> ctx) {
	context = ctx;
}

int TerminatedState::getDescriptor() const {
	return(-1);
}


IConnection::Status TerminatedState::send(std::string const& message, int flag) const {
	Packet packet;
	(void)flag;
	packet.payload = message;
	buffer.push_back(packet);

	return(IConnection::Status::ERROR);
}


IConnection::Status TerminatedState::send(Packet& packet, int flag) const {
	(void)flag;
	buffer.push_back(packet);
	return(IConnection::Status::ERROR);
}



IConnection::Status TerminatedState::recv(std::string& packet, int flag) {
	(void)flag;
	(void)packet;
	return(IConnection::Status::ERROR);
}

IConnection::Status TerminatedState::recv(Packet& packet, int flag) {
	(void)flag;
	(void)packet;
	return(IConnection::Status::ERROR);
}

