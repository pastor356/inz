#pragma once
#include "IStateContext.hpp"
#include "IConnectionHandler.hpp"


class MonitoredNode: public IStateContext {
	std::shared_ptr<IConnectionHandler> connection;

public:
	MonitoredNode() = default;
	MonitoredNode(std::shared_ptr<IConnectionHandler> initial);

	void changeState(std::shared_ptr<IConnectionHandler> newState) override;
	std::shared_ptr<IConnectionHandler> getHandler() const;
};
