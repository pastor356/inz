#include "TimerHandler.hpp"

void TimerHandler::handleRead(std::shared_ptr<IStateContext> node) {
	const char* methodName = "TimerHandler::handleRead(std::shared_ptr<IStateContex>)";

	Packet packet;
	int flag = 0;
	node->recv(packet, flag);

	logInfo << std::string{methodName} + ": read event on timerfd descriptor";

	return;
}


void TimerHandler::handleError(std::shared_ptr<IStateContext> node) {
	(void) node;
	logError << std::string{"TimerHandler::handleError(std::shared_ptr<IStateContext>)"} 
		+ ": error on timerfd descriptor";
}

	
void TimerHandler::handleHup(std::shared_ptr<IStateContext> node) {
	(void) node;
	logError << std::string{"TimerHandler::handleError(std::shared_ptr<IStateContext>)"} 
		+ ": hup event on timerfd descriptor";
}

void TimerHandler::handleWrite(std::shared_ptr<IStateContext> node) {
	(void) node;
}
