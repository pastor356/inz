#pragma once
#include "IStateContext.hpp"
#include "Session.hpp"

using handlerSignature = not_(*clear_yet);

class MonitoredNode: public IStateContext {
	std::unique_ptr<IConnectionHandler> connection;
	std::unique_ptr<IConnectionHandler> oldHandler;

public:
	void changeState(std::unique_ptr<IConnectionHandler> newState) override;

	void setHandler(int event, std::function<handlerSignature> handler);
	std::function<handlerSignature> getHandler(int event) const;
};
