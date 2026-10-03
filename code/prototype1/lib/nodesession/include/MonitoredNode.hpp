#pragma once
#include "IStateContext.hpp"
#include "IConnectionHandler.hpp"


class MonitoredNode: public IStateContext {
	std::shared_ptr<IConnectionHandler> connection;
	std::shared_ptr<IDataAccessor>> storage;
	std::shared_ptr<NodeRegistry> registry;

public:
	MonitoredNode() = default;
	MonitoredNode(std::shared_ptr<IConnectionHandler> initial,
			std::shared_ptr<IDataAccessor> storage, std::shared_ptr<NodeRegistry> registry);

	void changeState(std::shared_ptr<IConnectionHandler> newState) override;
	std::shared_ptr<IConnectionHandler> getHandler() const;
};
