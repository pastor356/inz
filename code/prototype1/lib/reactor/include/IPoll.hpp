#pragma once
#include "EventHandler.hpp"
#include "IStateContext.hpp"
#include <sys/epoll.h>

#define MAXEVENTS 100

class IPoll {
public:
	struct EventEntity {
		std::shared_ptr<IStateContext> node;
		std::shared_ptr<EventHandler> handler;
		int fd;
	};

	struct ReadyEvent {
		std::shared_ptr<EventEntity> entity;
		uint32_t flags;
	};

	virtual std::vector<IPoll::ReadyEvent> wait() const = 0;
	virtual void registerNode(std::shared_ptr<IStateContext> node,
			std::shared_ptr<EventHandler> handler) = 0;

	virtual void deregisterNode(std::shared_ptr<IStateContext> node) = 0;
	virtual void deregisterHandler(std::shared_ptr<EventHandler> handler) = 0;

	virtual ~IPoll() = default;
};
