#pragma once
#include "Session.hpp"
#include "Handshaker.hpp"
#include "DecoratorBuilder.hpp"
#include "MonitoredNode.hpp"
#include "InitialState.hpp"
#include "Serializer.hpp"
#include "ConsoleLogger.hpp"

class EventHandler {
public:
	virtual void handleRead(std::shared_ptr<IStateContext> node) = 0;
	virtual void handleError(std::shared_ptr<IStateContext> node) = 0;
	virtual void handleHup(std::shared_ptr<IStateContext> node) = 0;
	virtual void handleWrite(std::shared_ptr<IStateContext> node) = 0;
	virtual ~EventHandler() = default;
};
