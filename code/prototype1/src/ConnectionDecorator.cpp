#include "ConnectionDecorator.hpp"


IConnection::Status ConnectionDecorator::send(std::string const& msg, int flag) const {
	return(connection->send(msg, flag));
}

IConnection::Status ConnectionDecorator::recv(std::string& msg, int flag) const {
	return(connection->recv(msg, flag));
}



ConnectionDecorator::ConnectionDecorator(std::unique_ptr<IConnection>&& conn) {
	this->connection = std::move(conn);
}




