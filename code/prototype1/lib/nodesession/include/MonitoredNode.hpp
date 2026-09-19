#pragma once
#include "IStateContext.hpp"
#include "IConnectionHandler.hpp"


class MonitoredNode: public IStateContext {
	std::unique_ptr<IConnectionHandler> connection;

public:
	MonitoredNode(std::shared_ptr<IConnectionHandler> initial,
					std::shared_ptr<IHandlerFabric> fabric);

	void changeState(std::shared_ptr<IConnectionHandler> newState) override;
};
