#include "EchoRouter.hpp"

void EchoRouter::handleRead(std::shared_ptr<IStateContext> node) {
	const char* methodName = "EchoRouter::handleRead(std::shared_ptr<IStateContext>)";
	Packet packet;
	IConnection::Status status = IConnection::Status::SUCCESS;

	while(status == IConnection::Status::SUCCESS) {
		status = node->recv(packet);
	}

	std::cout << std::string{methodName} + " " + packet.payload << " with len " <<
	   packet.payload.size() << std::endl;
}


void EchoRouter::handleWrite(std::shared_ptr<IStateContext> node) {
	(void)node;
}


void EchoRouter::handleHup(std::shared_ptr<IStateContext> node) {
	if(auto strong = reactor.lock())
		strong->deregisterNode(node);

}

void EchoRouter::handleError(std::shared_ptr<IStateContext> node) {
	logInfo << "error while dispatching descriptor " + std::to_string(node->getDescriptor());
}

