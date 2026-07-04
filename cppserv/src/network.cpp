#pragma once
#include "network.hpp"


addrInfo resolveAddr(std::string const& hostname, std::string const& service, 
					int socktype, int flags = 0) {
    struct addrinfo hints, *res;
	std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = socktype;
    hints.ai_flags = flags;

    int err = getaddrinfo(hostname.empty() ? nullptr : hostname.c_str(),
                          service.empty() ? nullptr : service.c_str(),
                          &hints, &res);
    if (err != 0) {
        throw std::runtime_error("getaddrinfo: " + std::string(gai_strerror(err)));
    }

    return addrInfo(res);
}


SockRes createSocket(std::string const& hostname, 
					std::string const& service, int socktype, SockRole role) {
	int flags = (role == SockRole::Server? AI_PASSIVE : 0);
	int fd;
	struct sockaddr_storage addr;


	addrInfo ai = resolveAddr(hostname, service, socktype, flags);
	struct addrinfo *res = ai.get();

	do {
		fd = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if(fd < 0) continue;

		if(role == SockRole::Server) {
			::bind(fd, res->ai_addr, res->ai_addrlen);
			std::memcpy(&addr, res->ai_addr, res->ai_addrlen);
			
			return(SockRes{fd, addr, res->ai_addrlen});
		} else if(role == SockRole::Client) {
			if(::connect(fd, res->ai_addr, res->ai_addrlen) == 0) {
				std::memcpy(&addr, res->ai_addr, res->ai_addrlen);
				return(SockRes{fd, addr, res->ai_addrlen});
			}

		}

		close(fd);
	} while((res = res->ai_next) != nullptr);

	std::string ex(__PRETTY_FUNCTION__);
	ex += std::string{"failed: "} + strerror(errno);
	throw std::runtime_error(ex);
}


TcpConn::TcpConn(int fd, const struct sockaddr_storage addr, socklen_t addrlen):
	remoteFd(fd), addr(addr), addrlen(addrlen) {}

ssize_t TcpConn::send(std::string const& msg, int flags) const {
	if(msg.empty()) {
		std::string ex{__PRETTY_FUNCTION__};
		ex += std::string{": msg is empty\n"};
		throw std::runtime_error(ex); }

	size_t len = msg.size();

#ifndef DEBUG_CONSTR
	std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif

	return(::send(this->remoteFd, msg.data(), len, flags));
}


ssize_t TcpConn::recv(std::string& msg, int flags) const {
	size_t len = msg.capacity();
	char buf[len];
	
	ssize_t nrecv = ::recv(this->remoteFd, buf, len, flags);
	if(nrecv < 0) throw std::runtime_error{std::string{"recv failed"}};

	msg.assign(buf, nrecv);

	return(nrecv);
}

TcpConn::~TcpConn() {

	close(this->remoteFd);
}
