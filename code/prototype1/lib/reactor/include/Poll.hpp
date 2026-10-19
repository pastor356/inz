#pragma once
#include "IPoll.hpp"
#include <sys/epoll.h>

class Poll: public IPoll {
	int epollFd = -1;
	std::unordered_map<int, std::shared_ptr<IStateContext>> fdToNode;
	std::unordered_map<std::shared_ptr<IStateContext>, 
		std::shared_ptr<EventHandler>> nodeToHandler;

	std::unordered_map< std::shared_ptr<EventHandler>, 
		std::vector<std::shared_ptr<IStateContext>> > handlerToNodes;

public:
	Poll();
	void registerNode(std::shared_ptr<IStateContext> node, 
			std::shared_ptr<EventHandler> handler) override;

	void deregister(std::shared_ptr<EventHandler> handler) override; 
	void deregister(std::shared_ptr<IStateContext> node) override;

	size_t wait() const override;
};
