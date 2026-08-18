#include "TcpSocket.hpp"

TcpSocket::TcpSocket(int sockDescriptor, std::string const& address, int port): 
	sockFd{sockDescriptor} {
	this->addr = this->initAddress(address, port);
}



TcpSocket::TcpSocket(std::string const& address, int port) {
	this->addr = this->initAddress(address, port);

	this->sockFd = socket(this->addr.ss_family, SOCK_STREAM, 0);
	if(this->sockFd < 0) {
		logError << std::string{"socket() failed: "} + strerror(errno);
		throw std::runtime_error{"socket() failed"};
	} 

	int reuse = 1;
	setsockopt(this->sockFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
}



struct sockaddr_storage TcpSocket::initAddress(std::string const& address, int port) {
	int err;
	struct sockaddr_in ipv4;
	struct sockaddr_in6 ipv6;
	struct sockaddr_storage outputAddr;
	int isIpV4;
	
	std::memset(&outputAddr, 0, sizeof(outputAddr)); 

	isIpV4 = inet_pton(AF_INET, address.c_str(), &ipv4.sin_addr);

	if(isIpV4 < 0) 
		throw std::string{"inet_pton() failed: "} + strerror(errno);

	if(isIpV4) {
		ipv4.sin_family = AF_INET; 
		ipv4.sin_port = htons(port);

		std::memcpy(&outputAddr, &ipv4, sizeof(ipv4));

		return(outputAddr);

	} else {
		ipv6.sin6_family = AF_INET6;
		ipv6.sin6_port = htons(port);
		err = inet_pton(AF_INET6, address.c_str(), &ipv6.sin6_addr);

		if(err < 0) 
			throw std::string{"inet_pton() failed: "} + strerror(errno);

		std::memcpy(&outputAddr, &ipv6, sizeof(ipv6));

		return(outputAddr);
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
	int err = ::connect(this->sockFd, 
			reinterpret_cast<struct sockaddr*>(&ipAddr), sizeof(ipAddr));

	if(err < 0) {
		logError << std::string{"connect() failed: "} + strerror(errno);
		return(nullptr);
	}

	return(std::make_unique<TcpCallee>(this->sockFd, ipAddr));
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


TcpSocket::~TcpSocket() {
	char output[MAXLINE];
	
	if(addr.ss_family == AF_INET) {
		struct sockaddr_in* addrv4 = reinterpret_cast<struct sockaddr_in*>(&addr);

		logInfo << std::string{"Connection "} 
		+ std::string{inet_ntop(this->addr.ss_family, 
				&addrv4->sin_addr, output, sizeof(struct sockaddr_in))} << " closed :(";

	} else if(addr.ss_family == AF_INET6) {
		struct sockaddr_in6* addrv6 = reinterpret_cast<struct sockaddr_in6*>(&addr);

		logInfo << std::string{"Connection "} 
		+ std::string{inet_ntop(this->addr.ss_family, 
				&addrv6->sin6_addr, output, INET6_ADDRSTRLEN)} << " closed :(";
	}

	shutdown(this->sockFd, SHUT_RDWR);
}



std::unique_ptr<IConnection> TcpSocket::connect(addrInfo const& resolvedAddress) const  {
	struct addrinfo *iter = resolvedAddress.get();
	int err;

	do {

		if((err = ::connect(this->sockFd, iter->ai_addr, iter->ai_addrlen)) < 0) {
			logError << std::string{"connect() failed: "} + strerror(errno);
			continue;
		} else if(err == 0) break; // success
		
	} while((iter = iter->ai_next) != NULL);

	if(!iter) {
		logError << std::string{"connect() failed: "} + strerror(errno);
		struct sockaddr_storage failed;
		memset(&failed, 0, sizeof(failed));
		return(std::make_unique<TcpCallee>(-1, failed));
	}

	return(std::make_unique<TcpCallee>(this->sockFd, 
				*(reinterpret_cast<struct sockaddr_storage*>(iter->ai_addr))));
}

TcpSocket::TcpSocket(addrInfo const& resolvedAddress) { 
	struct addrinfo* iter = resolvedAddress.get();
	int err;

	if(iter == nullptr) {
		auto ex = std::string{__func__} + ": address info is corrupted";
		throw std::runtime_error{ex};
	}
	
	do {
		this->sockFd = ::socket(iter->ai_family, iter->ai_socktype, iter->ai_protocol);
		if(this->sockFd < 0) continue;

		int reuse = 1;
		setsockopt(this->sockFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));

		if((err = ::bind(this->sockFd, iter->ai_addr, iter->ai_addrlen)) == 0)
				break;

		close(this->sockFd);
		logWarn << std::string{"::bind() failed: "} + strerror(errno);
		std::cerr << std::string{"::bind() failed: "} + strerror(errno) << std::endl;


	} while((iter = iter->ai_next) != NULL);

	if(!iter) {
		logError << std::string{"socket creation failed: "} + strerror(errno);
	}


	this->addr = *(reinterpret_cast<struct sockaddr_storage*>(iter->ai_addr));
}


