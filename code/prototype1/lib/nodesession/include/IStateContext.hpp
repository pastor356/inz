#pragma once
#include "ISocket.hpp"
#include "IConnection.hpp"
#include "LiveMonitor.hpp"
#include "IQualityStrategy.hpp"
#include "IConnectionHandler.hpp"
#include <functional>
#include <memory>

class IStateContext {
public:
	virtual void changeState(std::shared_ptr<IConnectionHandler> newState) = 0;
};
