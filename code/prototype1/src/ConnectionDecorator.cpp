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

int ConnectionDecorator::getDescriptor() const {
	return(connection->getDescriptor());
}

const struct sockaddr_storage ConnectionDecorator::getAddr() const { 
	return(connection->getAddr());
}

IConnection::Status ConnectionDecorator::recv(Packet& packet, int flag) const {
	return(connection->recv(packet, flag));
}


IConnection::Status ConnectionDecorator::send(Packet& packet, int flag) const {
	return(connection->send(packet, flag));
}

