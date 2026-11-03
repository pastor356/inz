#pragma once
#include "IInvoker.hpp"
#include "IPoll.hpp"


class Reactor {
	std::shared_ptr<IPoll> demux;
	std::shared_ptr<IInvoker> dispatcher;

	volatile std::atomic<uint8_t> quitflag = 1;
public:
	Reactor(std::shared_ptr<IPoll> poller, std::shared_ptr<IInvoker> dispatcher);

	void registerNode(std::shared_ptr<IStateContext> node, 
			std::shared_ptr<EventHandler> handler);

	void deregisterNode(std::shared_ptr<IStateContext> node);
	void deregisterHandler(std::shared_ptr<EventHandler> handler);

	std::shared_ptr<EventHandler> getHandler() const;

	void runOnce();
	void run();
	void quit();
};
