#include "udp.hpp"


ssize_t UdpPeer::send(int servFd, std::string const& msg, size_t n, int flags) const {
	if(msg.empty()) {
		std::string ex{__PRETTY_FUNCTION__};
		ex += std::string(": empty msg");
		throw std::runtime_error(ex);
	}
#ifndef DEBUG
	std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif

	return(::sendto(servFd, msg.c_str(), n, flags,(struct sockaddr*)&this->remoteAddr, this->addrlen));
}

ssize_t UdpPeer::recv(int servFd, std::string & msg, size_t n, int flags) const {
	if(msg.capacity() < n) {
		msg.resize(n);
	}

#ifndef DEBUG
	std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
	struct sockaddr unspec;
	std::memset(&unspec, 0, sizeof(struct sockaddr));
	unspec.sa_family = AF_UNSPEC;

	::connect(servFd, (struct sockaddr*)&this->remoteAddr, this->addrlen);
	ssize_t nrecv = ::recv(servFd, msg.data(), n, MSG_DONTWAIT | flags);
	::connect(servFd, &unspec, sizeof(struct sockaddr));

	return(nrecv);
}



void UdpPeer::setAddr(std::string const& addr) {
	addrInfo res = resolveAddr(addr, nullptr, SOCK_DGRAM, 0);
	struct addrinfo *ai = res.get();

	std::memcpy(&this->remoteAddr, res->ai_addr, res->ai_addrlen);
	this->addrlen = res->ai_addrlen;

	return;
}

void UdpPeer::setAddr(struct sockaddr_storage &addr, socklen_t addrlen) {
	std::memcpy(&addr, &this->remoteAddr, addrlen);
	this->addrlen = addrlen;

	return;
}


UdpPeer::UdpPeer() {}


UdpPeer::UdpPeer(struct sockaddr_storage &addr, socklen_t addrlen): addrlen{addrlen} {
	std::memcpy(&addr, &this->remoteAddr, this->addrlen);
}


UdpPeer::UdpPeer(std::string const& hostname, std::string const& service, SockRole role) { 
	SockRes sock = createSocket(hostname, service, SOCK_DGRAM, role);
	int reuse = 1;
	setsockopt(sock.fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));

	std::memcpy(&this->remoteAddr, &sock.addr, sock.addrlen);
	this->addrlen = sock.addrlen;
}

