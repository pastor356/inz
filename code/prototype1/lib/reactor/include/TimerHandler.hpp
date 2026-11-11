#pragma once
#include "ITimerQueue.hpp"
#include "EventHandler.hpp"
#include "IStateContext.hpp"

class TimerHandler: public EventHandler {
public:
	void handleRead(std::shared_ptr<IStateContext> node) override;
	void handleError(std::shared_ptr<IStateContext> node) override;
	void handleHup(std::shared_ptr<IStateContext> node)  override;
	void handleWrite(std::shared_ptr<IStateContext> node) override;
};
