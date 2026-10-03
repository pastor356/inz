#include "IConnectionHandler.hpp"
#include "MonitoredNode.hpp"
#include "IStateContext.hpp"

void MonitoredNode::changeState(std::shared_ptr<IConnectionHandler> newState) {
	connection = newState;
}


MonitoredNode::MonitoredNode(std::shared_ptr<IConnectionHandler> initial):
	connection{initial} {}

std::shared_ptr<IConnectionHandler> MonitoredNode::getHandler() const {
	return(connection);
}


MonitoredNode(std::shared_ptr<IConnectionHandler> initial,
		std::shared_ptr<IDataAccessor> storage, std::shared_ptr<NodeRegistry> registry):
	connection{initial}, storage{storage}, registry{registry} {}
