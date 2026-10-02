#include "DecoratorBuilder.hpp"

void DecoratorBuilder::setConnection(std::unique_ptr<IConnection> conn) {
	connection = std::move(conn);
}

void DecoratorBuilder::setQos1() {
	connection = std::make_unique<Qos1>(std::move(connection));
}

void DecoratorBuilder::setLiveMon() {
	connection = std::make_unique<LiveMonitor>(std::move(connection));
}

std::unique_ptr<IConnection> DecoratorBuilder::getDecorator() {
	return(std::move(connection));
}


