#include "LiveMonitor.hpp"


IConnection::Status LiveMonitor::recv(std::string& msg, int flag) const {

	while(true) {
		auto status = connection->recv(msg, flag);

		if(IConnection::Status::SUCCESS != status) return(status);

		resetTimer();

		if(isKeepAlive(msg)) continue;
	}
}


void LiveMonitor::resetTimer() const {
	lastActivity = std::chrono::steady_clock::now();
}


bool LiveMonitor::isSessionExpired() const {
	return((lastActivity - std::chrono::steady_clock::now()) > timeout);
}



bool LiveMonitor::isKeepAlive(std::string const& msg) const {


	return(false);
}
