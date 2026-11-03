#pragma once
#include "AcceptorSocket.hpp"
#include "MessageRouter.hpp"
#include "IPoll.hpp"

class NodeInitializer: public EventHandler {
	std::weak_ptr<Reactor> reactor;
	std::shared_ptr<EventHandler> messageRouter;
	uint32_t counter = 0;

public:
	NodeInitializer(std::shared_ptr<Reactor> poll, std::shared_ptr<EventHandler> messRouter);

	void handleRead(std::shared_ptr<IStateContext> node) override;
	void handleWrite(std::shared_ptr<IStateContext> node) override;
	void handleError(std::shared_ptr<IStateContext> node) override;
	void handleHup(std::shared_ptr<IStateContext> node) override;
};

