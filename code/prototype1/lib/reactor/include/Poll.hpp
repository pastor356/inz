#pragma once
#include "IPoll.hpp"
#include <sys/epoll.h>

class Poll: public IPoll {
	int epollFd = -1;
	int pollTimeout = -1;

	std::unordered_map<int, std::shared_ptr<EventEntity>> fdToEntity;
	std::unordered_map<std::shared_ptr<IStateContext>, int> nodeToFd;

	void removeFromFdToEntity(int fd);

public:
	Poll(int timeout = -1);

	void registerNode(std::shared_ptr<IStateContext> node, 
			std::shared_ptr<EventHandler> handler) override;

	void deregisterHandler(std::shared_ptr<EventHandler> handler) override; 
	void deregisterNode(std::shared_ptr<IStateContext> node) override;

	int wait() override;
};
