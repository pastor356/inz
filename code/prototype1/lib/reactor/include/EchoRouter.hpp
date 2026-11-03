#pragma once
#include "Reactor.hpp"
#include "EventHandler.hpp"

class EchoRouter: public EventHandler {
	std::weak_ptr<Reactor> reactor;

public:
	EchoRouter(std::shared_ptr<Reactor> poller): reactor{poller} {}

	void handleRead(std::shared_ptr<IStateContext> node) override;
	void handleWrite(std::shared_ptr<IStateContext> node) override;
	void handleHup(std::shared_ptr<IStateContext> node) override;
	void handleError(std::shared_ptr<IStateContext> node) override;
};


