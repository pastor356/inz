#include "tcp.hpp"

TcpClient::TcpClient(std::string const& hostname, std::string const& service):
	hostname{hostname}, service{service} {
}


// refactor with resolveAddr() and createSocket()
std::unique_ptr<TcpConn> TcpClient::connect() const {
	struct addrinfo *res, *ressave, hints;
	int err;
	int clientFd;

	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if((err = getaddrinfo(this->hostname.data(), this->service.data(), &hints, &res)) != 0) {
		std::string ex{__PRETTY_FUNCTION__};
		ex += std::string{": getaddrinfo() failed: "};
		ex += gai_strerror(err);
		throw std::runtime_error{ex};
	}

	ressave = res;

	do { 
		clientFd = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if(clientFd < 0) continue;

		if(::connect(clientFd, res->ai_addr, res->ai_addrlen) >= 0) 
			break;

	} while((res = res->ai_next) != NULL);

	if(res == NULL) {
		std::string ex{__PRETTY_FUNCTION__};
		ex += std::string{": socket() failed: "};
		ex += strerror(errno);
		throw std::runtime_error{ex};
	}

	struct sockaddr_storage serv;
	std::memset(&serv, 0, sizeof(struct sockaddr_storage));
	std::memcpy(&serv, res->ai_addr, res->ai_addrlen);

	std::unique_ptr<TcpConn> conn = 
		std::unique_ptr<TcpConn>(new TcpConn(clientFd, serv, res->ai_addrlen));

	freeaddrinfo(ressave);
	return(conn);
}


int TcpAcc::listen(int backlog) const {

#ifdef DEBUG_CONSTR
	std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif

	return(::listen(this->servFd, backlog));
}

TcpAcc::TcpAcc(std::string const& hostname, std::string const& service) {
	if(hostname.empty() && service.empty()) {
		throw std::runtime_error("both hostname and servicename are empty");
	}

	int reuse = 1;

	SockRes sock = createSocket(hostname, service, SOCK_STREAM, SockRole::Server);
	setsockopt(sock.fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
	this->servFd = sock.fd;
	this->selfAddr = sock.addr;
	this->addrlen = sock.addrlen;
}


std::unique_ptr<TcpConn> TcpAcc::accept() const {
	struct sockaddr_storage addr;
	socklen_t addrlen = sizeof(struct sockaddr_storage);
	std::memset(&addr, 0, addrlen);

	int clientFd;
	if((clientFd = ::accept(this->servFd, 
					reinterpret_cast<struct sockaddr*>(&addr), 
					&addrlen)) < 0) {
		std::string ex{__PRETTY_FUNCTION__};
		ex += std::string(": accept() failed: ");
		ex += strerror(errno);
		throw std::runtime_error(ex);
	}

	// make_unique is not a friend of TcpConn
	// so it can't call private constructor
	return(std::unique_ptr<TcpConn>(new TcpConn(clientFd, addr, addrlen)));
}


TcpAcc::~TcpAcc() {
	if(servFd > 0) close(this->servFd);
}

