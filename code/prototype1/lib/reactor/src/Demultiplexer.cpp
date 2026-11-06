#include "Demultiplexer.hpp"

Demultiplexer::Demultiplexer(int timeout): epollTimeout{timeout} {
	const char* methodName = "Demultiplexer::Demultiplexer(int)";

	this->waiter = std::make_unique<EPollWaiter>();
	this->epollFd = waiter->create();
}

Demultiplexer::Demultiplexer(std::unique_ptr<IWaiter> wait, int timeout):
	waiter{std::move(wait)} {
	
	const char* methodName = "Demultiplexer::Demultiplexer(std::unique_ptr<IWaiter>, int)";
	this->epollFd = waiter->create();
}

void Demultiplexer::registerNode(std::shared_ptr<IStateContext> node,
		std::shared_ptr<EventHandler> handler) {
	const char* methodName = 
		"Demultiplexer::registerNode(std::shared_ptr<IStateContext>, std::shared_ptr<EventHandler>)";
	
	struct epoll_event ev;
	ev.data.fd = node->getDescriptor();
	ev.events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLRDHUP;

	auto entity = std::make_shared<EventEntity>(EventEntity{node, handler, node->getDescriptor()});

	waiter->ctl(this->epollFd, EPOLL_CTL_ADD, node->getDescriptor(), &ev);

	auto iter = fdToEntity.try_emplace(node->getDescriptor(), std::move(entity));
	if(!iter.second) {
		logError << std::string{methodName} + " failed to emplace entity";
	}

	auto it = nodeToFd.try_emplace(node, node->getDescriptor());
	if(!it.second) {
		logError << std::string{methodName} + " failed to emplace entity";
	}


	logInfo << std::string{methodName} + ": node registered";
}


void Demultiplexer::deregisterNode(std::shared_ptr<IStateContext> node) {
	const char* methodName = "Demultiplexer::deregisterNode(std::shared_ptr<IStateContext>)";

	int fdToRemove = node->getDescriptor();

	logInfo << std::string{methodName} + " node deregistered";

	removeFromFdToEntity(fdToRemove);

	auto it = nodeToFd.find(node);
	if(it != nodeToFd.end()) {
		nodeToFd.erase(it);

		if(waiter->ctl(epollFd, EPOLL_CTL_DEL, node->getDescriptor(), nullptr) < 0) {
			logError << std::string{methodName} + " failed: epoll_ctl(EPOLL_CTL_DEL) failed";
		}

	} else {
		logError << std::string{methodName} + " failed to erase node with fd " + 
			std::to_string(node->getDescriptor());
	}
}


void Demultiplexer::deregisterHandler(std::shared_ptr<EventHandler> handler) {
	const char* methodName = "Poll::deregisterHandler(std::shared_ptr<EventHandler>)";

	std::vector<IPoll::EventEntity> entityToRemove;
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

std::vector<IPoll::ReadyEvent> Demultiplexer::wait() const {
	const char* methodName = "Demultiplexer::wait()";
	std::vector<IPoll::ReadyEvent> ret;
	struct epoll_event events[MAXEVENTS];

	int nevents = waiter->wait(this->epollFd, events, MAXEVENTS, this->epollTimeout);

	logInfo << std::string{methodName} + ": epoll_wait() returned " + std::to_string(nevents);

	if(nevents < 0) {
		logError << std::string{methodName} + " failed: epoll_wait() < 0";
	}

	for(int i = 0; i < nevents; ++i) {
		auto it = fdToEntity.find(events[i].data.fd);

		if(it != fdToEntity.end()) {
			auto readyEvent = IPoll::ReadyEvent{it->second, events[i].events};
			ret.push_back(std::move(readyEvent));
		} else {
			logError << std::string{methodName} + " failed: handler for "
				+ std::to_string(events[i].data.fd) + " not found";
		}
	}

	return(ret);
}


void Demultiplexer::removeFromFdToEntity(int fdToRemove) {
	const char* methodName = "Poll::removeFromFdToEntity(int)";

	auto iter = fdToEntity.find(fdToRemove);

	if(iter != fdToEntity.end()) 
		fdToEntity.erase(iter);
	else 
		logError << std::string{methodName} + " failed to erase node with fd " + 
			std::to_string(fdToRemove);

	return;
}

