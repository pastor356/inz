#include "NodeInitializer.hpp"


NodeInitializer::NodeInitializer(std::shared_ptr<Reactor> poll, 
		std::shared_ptr<EventHandler> messRouter): 
	reactor{poll}, messageRouter{messRouter} {}
	

void NodeInitializer::handleRead(std::shared_ptr<IStateContext> node) {
	const char* methodName = "NodeInitializer::handleRead(std::shared_ptr<IStateContext>)";
	Packet packet;


	auto status = node->recv(packet, 0);
	counter++;

	if(node->isInitialized()) {
		if(auto r = reactor.lock()) {
			r->deregisterNode(node);
			r->registerNode(node, this->messageRouter);
		}
	}

	if(status != IConnection::Status::SUCCESS) {
		logError << std::string{methodName} + " failed: status != SUCCESS";
	}
}




void NodeInitializer::handleWrite(std::shared_ptr<IStateContext> node) {
	(void) node;
}

void NodeInitializer::handleHup(std::shared_ptr<IStateContext> node) {
	(void) node;
}

void NodeInitializer::handleError(std::shared_ptr<IStateContext> node) {
	(void) node;
}
