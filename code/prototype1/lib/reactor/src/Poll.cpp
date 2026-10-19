#include "Poll.hpp"

Poll::Poll() {
	const char* methodName = "Poll::Poll()";
	epollFd = epoll_create(1);
	if(epollFd < 0) {
		logError << std::string{methodName} + " failed: " + strerror(epollFd);
	}
}

void Poll::registerNode(std::shared_ptr<IStateContext> node, 
						std::shared_ptr<EventHandler> handler) {
	const char* methodName = 
		"Poll::registerNode(std::shared_ptr<IStateContext>, std::shared_ptr<EventHandler>)";

	auto itF2N = fdToNode.try_emplace(node->getDescriptor(), node);
	if(!itF2N.second) {
		logError << std::string{methodName} + " failed: emplasing into fdToNode failed";
		return;
	}

	auto itN2H = nodeToHandler.try_emplace(node, handler);
	if(!itN2H.second) {
		logError << std::string{methodName} + " failed: emplasing into nodeToHandler failed";
		return;
	}

	auto itH2N = handlerToNodes.find(handler);
	if(itH2N != handlerToNodes.end()) 
		itH2N->second.push_back(std::move(node));
	else 
		logError << std::string{methodName} + " failed: emplasing into handerToNodes failed";

	return;
}

void Poll::deregister(std::shared_ptr<IStateContext> node) {
	const char* methodName = "Poll::deregister(std::shared_ptr<IStateContext>)";



