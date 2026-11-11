#include "AcceptorSocket.hpp"


AcceptorSocket::AcceptorSocket(std::shared_ptr<Reactor> poll, 
		std::shared_ptr<IDataAccessor> stor, std::shared_ptr<NodeRegistry> reg, 
		std::shared_ptr<EventHandler> nodeInit):
	reactor{poll}, storage{stor}, registry{reg}, nodeInitializer{nodeInit} {}

AcceptorSocket::AcceptorSocket(std::shared_ptr<Reactor> poll, 
		std::shared_ptr<IDataAccessor> stor, std::shared_ptr<NodeRegistry> reg, 
		std::shared_ptr<EventHandler> nodeInit,
		std::shared_ptr<TimerQueue> timer):
	reactor{poll}, storage{stor}, registry{reg}, nodeInitializer{nodeInit}, timer{timer} {}


void AcceptorSocket::handleRead(std::shared_ptr<IStateContext> node) {
	auto socket = std::dynamic_pointer_cast<AcceptorNode>(node);

	std::unique_ptr<IConnection> conn = socket->accept();

	auto connHandler = std::make_shared<InitialState>(std::move(conn));

	auto monitoredNode = std::make_shared<MonitoredNode>(connHandler, storage, registry, timer);

	connHandler->setContext(monitoredNode);

	if(auto r = reactor.lock()) 
		r->registerNode(monitoredNode, nodeInitializer);
}



void AcceptorSocket::handleWrite(std::shared_ptr<IStateContext> node) {
	(void)node;
}
void AcceptorSocket::handleError(std::shared_ptr<IStateContext> node) {
	(void)node;
}
void AcceptorSocket::handleHup(std::shared_ptr<IStateContext> node) {
	(void)node;
}



