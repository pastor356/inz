#include "TcpConnection.hpp"

TcpConnection::TcpConnection(int sockFd, struct sockaddr_storage addr): 
	connectionFd{sockFd}, addr{addr} {}


IConnection::Status TcpConnection::recv(std::string& msg, int flag) const {
	ssize_t nrecv;
	char buffer[MAXMSGSIZ];

	nrecv = ::recv(connectionFd, buffer, MAXMSGSIZ, flag);
	if(nrecv < 0) {
		logError << std::string{"recv() failed: "} + std::string{strerror(errno)};
		return(IConnection::Status::ERROR);
	} else if(nrecv == 0) {
		return(IConnection::Status::HUP);
	}


	msg = std::string{buffer, buffer + nrecv};

	return(IConnection::Status::SUCCESS);
}


IConnection::Status TcpConnection::send(std::string const& msg, int flag) const {
	ssize_t nsend;

	if(msg.size() > MAXMSGSIZ) {
		logError << std::string{"send(): max message size overflow"};
	}

	nsend = ::send(connectionFd, msg.c_str(), msg.size(), flag);
	if(nsend < 0) {
		logError << std::string{"send() failed: "} + std::string{strerror(errno)};
		return(IConnection::Status::ERROR);
	} 

	return(IConnection::Status::SUCCESS);
}


const struct sockaddr_storage TcpConnection::getAddr() const {
	return(addr);
}


int TcpConnection::getDescriptor() const {return(this->connectionFd);}


std::string TcpConnection::sock_ntop() const {
	char res[MAXLINE];

	switch(addr.ss_family) {
		case AF_INET: {
			auto *sin = reinterpret_cast<const struct sockaddr_in*>(&this->addr);

			if(inet_ntop(AF_INET, &sin->sin_addr, res, MAXLINE) != NULL) {
					return(std::string{res});
			}

		} case AF_INET6: {
			 auto *sin6 = reinterpret_cast<const struct sockaddr_in6*>(&this->addr);

			if(inet_ntop(AF_INET6, &sin6->sin6_addr, res, MAXLINE) != NULL) {
					return(std::string{res});
			}

		} default: {
			printf("%s: unknown sa_family\n", __func__);
			return(std::string{});
		}
	}
}




TcpConnection::~TcpConnection() {
	shutdown(connectionFd, SHUT_RDWR);
	std::string log{"connection "};

	log += this->sock_ntop();
	log += " closed";
}
