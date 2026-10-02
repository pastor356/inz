#pragma once
#include "ISocket.hpp"
#include "IConnection.hpp"
#include "LiveMonitor.hpp"
#include "IConnectionHandler.hpp"
#include "IQualityStrategy.hpp"
#include <functional>
#include <memory>

class IConnectionHandler;

class IStateContext {
public:
	virtual void changeState(std::shared_ptr<IConnectionHandler> newState) = 0;
};
