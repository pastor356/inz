#pragma once
#include "IPoll.hpp"
#include "MessageRouter.hpp"
#include "Reactor.hpp"
#include "AcceptorNode.hpp"

class AcceptorSocket: public EventHandler {
	std::weak_ptr<Reactor> reactor;

	std::shared_ptr<IDataAccessor> storage;
	std::shared_ptr<NodeRegistry> registry;

	std::shared_ptr<EventHandler> nodeInitializer;
public: 
	AcceptorSocket(std::shared_ptr<Reactor> poll, 
			std::shared_ptr<IDataAccessor> stor, std::shared_ptr<NodeRegistry> reg,
			std::shared_ptr<EventHandler> nodeInit);

	void handleRead(std::shared_ptr<IStateContext> node) override;
	void handleWrite(std::shared_ptr<IStateContext> node) override;
	void handleHup(std::shared_ptr<IStateContext> node) override;
	void handleError(std::shared_ptr<IStateContext> node) override;
};
