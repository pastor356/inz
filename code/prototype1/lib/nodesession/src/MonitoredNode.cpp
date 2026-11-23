#include "IConnectionHandler.hpp"
#include "MonitoredNode.hpp"
#include "IStateContext.hpp"
#include "Handshaker.hpp"

void MonitoredNode::changeState(std::shared_ptr<IConnectionHandler> newState) {
	connection = newState;
}

IConnectionHandler* MonitoredNode::getHandler() const {
	return(connection.get());
}


MonitoredNode::MonitoredNode(std::shared_ptr<IConnectionHandler> initial,
		std::shared_ptr<IDataAccessor> storage, std::shared_ptr<NodeRegistry> registry):
	connection{initial}, oldConn{nullptr}, storage{storage}, registry{registry} {}


MonitoredNode::MonitoredNode(std::shared_ptr<IConnectionHandler> initial,
		std::shared_ptr<IDataAccessor> storage, 
		std::shared_ptr<NodeRegistry> registry,
		std::shared_ptr<TimerQueue> timer): 
	connection{initial}, oldConn{nullptr}, storage{storage}, registry{registry}, 
	timer{timer} {}


void MonitoredNode::changeStateHandshake(std::unique_ptr<IConnection> conn,
		std::shared_ptr<IStateContext> ctx) {

	oldConn = std::move(connection);
	connection = std::make_shared<Handshaker>(ctx, std::move(conn), storage, registry, timer);
}



void MonitoredNode::changeStateReconnect(std::unique_ptr<IConnection> conn,
		std::shared_ptr<IStateContext> ctx) {

	oldConn = std::move(connection);
	connection = 
		std::make_shared<ReconnectState>(std::move(conn), ctx, registry, storage);
}


void MonitoredNode::changeStateSession(std::unique_ptr<IConnection> conn, 
		std::shared_ptr<IStateContext> ctx, UserAuth userAuth) {

	oldConn = std::move(connection);
	auto session = 
		std::make_shared<Session>(std::move(conn), ctx, userAuth, timer);

	registry->addSession(session->getId(), userAuth);
	connection = std::move(session);
}

void MonitoredNode::changeStateTerminated(std::shared_ptr<IStateContext> ctx, 
		UserAuth authUser) {

	oldConn = std::move(connection);
	connection = std::make_shared<TerminatedState>(ctx, authUser);
}


IConnection::Status MonitoredNode::recv(Packet& packet, int flag) {
	return(connection->recv(packet, flag));
}


IConnection::Status MonitoredNode::send(Packet& packet, int flag) {
	return(connection->send(packet, flag));
}

void MonitoredNode::setRole(uint8_t role) {
	this->role = role; 
}


bool MonitoredNode::isConsumer() const {
	if((role & ROLE_CONS) == ROLE_CONS) return(true);

	return(false);
}

int MonitoredNode::getDescriptor() const {
	return(connection->getDescriptor());
}
