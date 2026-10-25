#include "Reactor.hpp"

Reactor::Reactor(std::shared_ptr<IPoll> poller, std::shared_ptr<IInvoker> dispatcher):
	demux{poller}, dispatcher{dispatcher} {}


void Reactor::registerNode(std::shared_ptr<IStateContext> node,
		std::shared_ptr<EventHandler> handler) {

	demux->registerNode(node, handler);
}

void Reactor::deregisterHandler(std::shared_ptr<EventHandler> handler) {
	demux->deregisterHandler(handler);
}

void Reactor::deregisterNode(std::shared_ptr<IStateContext> node) {
	demux->deregisterNode(node);
}

void Reactor::runOnce() {
	auto eventsVector = demux->wait();
	dispatcher->dispatch(eventsVector);
}

void Reactor::run() {
	while(quitflag) {
		this->runOnce();
	}
}
		




