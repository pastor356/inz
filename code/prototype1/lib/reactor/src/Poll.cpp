#include "Poll.hpp"

Poll::Poll(int timeout) {
	const char* methodName = "Poll::Poll()";
	epollFd = epoll_create(1);
	if(epollFd < 0) {
		logError << std::string{methodName} + " failed: " + strerror(epollFd);
	}

	this->pollTimeout = timeout;
}



void Poll::registerNode(std::shared_ptr<IStateContext> node, 
			std::shared_ptr<EventHandler> handler) {
	const char* methodName = 
		"Poll::registerNode(std::shared_ptr<IStateContext>, std::shared_ptr<EventHandler>)";
	
	struct epoll_event ev;
	ev.data.fd = node->getDescriptor();
	ev.events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP;

	auto entity = std::make_unique<EventEntity>(EventEntity{node, handler, node->getDescriptor()});

	epoll_ctl(this->epollFd, EPOLL_CTL_ADD, node->getDescriptor(), &ev);

	auto iter = fdToEntity.try_emplace(node->getDescriptor(), std::move(entity));
	if(!iter.second) {
		logError << std::string{methodName} + " failed to emplace entity";
	}

	auto it = nodeToFd.try_emplace(node, node->getDescriptor());
	if(!it.second) {
		logError << std::string{methodName} + " failed to emplace entity";
	}

	return;
}



void Poll::deregisterNode(std::shared_ptr<IStateContext> node) {
	const char* methodName = "Poll::deregisterNode(std::shared_ptr<IStateContext>)";

	int fdToRemove = node->getDescriptor();

	removeFromFdToEntity(fdToRemove);

	auto it = nodeToFd.find(node);
	if(it != nodeToFd.end()) {
		nodeToFd.erase(it);

		if(epoll_ctl(epollFd, EPOLL_CTL_DEL, node->getDescriptor(), nullptr) < 0) {
			logError << std::string{methodName} + " failed: epoll_ctl(EPOLL_CTL_DEL) failed";
		}

	} else {
		logError << std::string{methodName} + " failed to erase node with fd " + 
			std::to_string(node->getDescriptor());
	}
}



void Poll::deregisterHandler(std::shared_ptr<EventHandler> handler) {
	const char* methodName = "Poll::deregisterHandler(std::shared_ptr<EventHandler>)";

	std::vector<EventEntity> entityToRemove;
	entityToRemove.reserve(30);
	std::vector<std::shared_ptr<IStateContext>> nodesToRemove;
	
	for(auto i = fdToEntity.begin(); i != fdToEntity.end(); ++i) {
		if(i->second && i->second->handler == handler)
			nodesToRemove.push_back(i->second->node);
	}

	for(size_t i = 0; i < nodesToRemove.size(); ++i) {
		this->deregisterNode(nodesToRemove[i]);
	}

	return;
}


int Poll::wait() {
	const char* methodName = "Poll:wait()";
	struct epoll_event events[MAXEVENTS];
	int res = epoll_wait(epollFd, events, MAXEVENTS, this->pollTimeout);

	if(res < 0) {
		logError << std::string{methodName} + " failed: epoll_wait() < 0";
	}

	std::shared_ptr<EventEntity> eventEntity;
	int eventFd;
	int flags;

	for(int i = 0; i < res; ++i) {

		eventFd = events[i].data.fd;
		auto it = fdToEntity.find(eventFd);
		
		if(it != fdToEntity.end()) continue;

		if(!it->second || !it->second->handler || !it->second->node) {
			eventEntity = it->second;
			flags = events[i].events;

			if(flags & EPOLLHUP)
				eventEntity->handler->handleHup(eventEntity->node);

			if(flags & EPOLLERR)
				eventEntity->handler->handleError(eventEntity->node);

			if(flags & EPOLLIN)
				eventEntity->handler->handleRead(eventEntity->node);

			if(flags & EPOLLOUT)
				eventEntity->handler->handleWrite(eventEntity->node);

		} 

	} // for(size_t
	
	return(res);
}



void Poll::removeFromFdToEntity(int fdToRemove) {
	const char* methodName = "Poll::removeFromFdToEntity(int)";

	auto iter = fdToEntity.find(fdToRemove);

	if(iter != fdToEntity.end()) 
		fdToEntity.erase(iter);
	else 
		logError << std::string{methodName} + " failed to erase node with fd " + 
			std::to_string(fdToRemove);

	return;
}
