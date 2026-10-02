#include "SessionBuilder.hpp"

std::shared_ptr<Session> SessionBuilder::getSession() const {
	return(result);
}

void SessionBuilder::reset() {
	this->result = std::make_shared<Session>();
}

void SessionBuilder::setConnection(std::unique_ptr<IConnection> conn) {
	result->setConnection(std::move(conn));
}

void SessionBuilder::setContext(std::shared_ptr<IStateContext> ctx) {
	result->setContext(ctx);
}


