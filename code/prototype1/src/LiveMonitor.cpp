#include "LiveMonitor.hpp"


IConnection::Status LiveMonitor::recv(Packet& packet, int flag) const {
	IConnection::Status status = IConnection::Status::AGAIN;

	while(1) {
		status = connection->recv(packet, flag);

		if(status != IConnection::Status::SUCCESS) return(status);

		resetTimer();

		if(isKeepAlive(packet)) continue;

		return(IConnection::Status::SUCCESS);
	}
}



IConnection::Status LiveMonitor::recv(std::string& msg, int flag) const {
	IConnection::Status status = IConnection::Status::AGAIN;
	Packet packet;

	status = this->recv(packet, flag);
	msg = packet.payload;

	return(status);
}


void LiveMonitor::resetTimer() const {
	lastActivity = std::chrono::steady_clock::now();
}


bool LiveMonitor::isSessionExpired() const {
	return((lastActivity - std::chrono::steady_clock::now()) > timeout);
}



bool LiveMonitor::isKeepAlive(Packet& packet) const {
	if((packet.header[0] & MSGTYPE_KEEPALIVE) == MSGTYPE_KEEPALIVE) return(true);

	return(false);
}
