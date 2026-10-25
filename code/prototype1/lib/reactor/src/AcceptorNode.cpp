#include "AcceptorNode.hpp"

AcceptorNode::AcceptorNode(std::unique_ptr<ISocket> sock): socket{std::move(sock)} {}

std::unique_ptr<IConnection> AcceptorNode::accept() const {
	return(socket->accept());
}

int AcceptorNode::getDescriptor() const {
	return(socket->getDescriptor());
}
