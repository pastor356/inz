#include "TcpSocket.hpp"


TcpSocket::TcpSocket(int sockDescriptor, std::string const& address, int port): 
	sockFd{sockDescriptor} {
	this->initAddress(address, port);
}



void TcpSocket::initAddress(std::string const& address, int port) {
	std::memset(&(this->addr), 0, sizeof(this->addr)); 
	int err; 
	struct sockaddr_in ipv4;
	struct sockaddr_in6 ipv6;
	int isIpV4;

	isIpV4 = inet_pton(AF_INET, address.c_str(), &ipv4.sin_addr);

	if(isIpV4 < 0) 
		throw std::string{"inet_pton() failed: "} + strerror(errno);

	if(isIpV4) {
		ipv4.sin_family = AF_INET; 
		ipv4.sin_port = htons(port);

		std::memcpy(&this->addr, &ipv4, sizeof(ipv4));

	} else {
		ipv6.sin6_family = AF_INET6;
		ipv6.sin6_port = htons(port);
		err = inet_pton(AF_INET6, address.c_str(), &ipv6.sin6_addr);

		if(err < 0) 
			throw std::string{"inet_pton() failed: "} + strerror(errno);

		std::memcpy(&this->addr, &ipv6, sizeof(ipv6));
	}
}



std::unique_ptr<IConnection> TcpSocket::accept() const {
	struct sockaddr_storage remoteAddr;
	socklen_t remoteAddrlen = sizeof(remoteAddr);
	int remoteFd;

	std::memset(&remoteAddr, 0, sizeof(remoteAddr));
	remoteFd = ::accept(this->sockFd, 
			reinterpret_cast<struct sockaddr*>(&remoteAddr), &remoteAddrlen);

	if(remoteFd < 0) {
		logError << std::string{"accept() failed: "} + strerror(errno);
		return(nullptr);
	}

	return(std::make_unique<TcpCaller>(remoteFd, remoteAddr));
}




std::unique_ptr<IConnection> TcpSocket::connect(struct sockaddr_storage ipAddr) const {
	int remoteFd = ::connect(this->sockFd, 
			reinterpret_cast<struct sockaddr*>(&ipAddr), sizeof(ipAddr));

	if(remoteFd < 0) {
		logError << std::string{"connect() failed: "} + strerror(errno);
		return(nullptr);
	}

	return(std::make_unique<TcpCallee>(remoteFd, ipAddr));
}



void TcpSocket::bind() const {
	
	socklen_t addrLen = (addr.ss_family == AF_INET)? 
		sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);

	int err = ::bind(this->sockFd, reinterpret_cast<const sockaddr*>(&addr), addrLen);

	if(err < 0) {
		logError << std::string{"bind() failed: "} + strerror(errno);
	}
	 
	return;
}



void TcpSocket::listen(int backlog) const {
	int err = ::listen(this->sockFd, backlog);

	if(err < 0) {
		logError << std::string{"listen() failed: "} + strerror(errno);
	}

	return;
}


int TcpSocket::getDescriptor() const {
	return(this->sockFd);
}


