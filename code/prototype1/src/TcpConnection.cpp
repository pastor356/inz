#include "TcpConnection.hpp"

TcpConnection::TcpConnection(int sockFd, struct sockaddr_storage addr): 
	connectionFd{sockFd}, addr{addr} {}


IConnection::Status TcpConnection::recv(std::string& msg, int flag) const {
	ssize_t nrecv;
	char buffer[MAXMSGSIZ];

	nrecv = ::recv(connectionFd, buffer, MAXMSGSIZ, flag);
	if(nrecv < 0) {
		if(errno == EAGAIN) return(IConnection::Status::AGAIN);

		logError << std::string{"recv() failed: "} + strerror(errno);

		if(errno == EAGAIN) return(IConnection::Status::AGAIN);

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
		if(errno == EAGAIN) return(IConnection::Status::AGAIN);
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

			break;
		} case AF_INET6: {
			 auto *sin6 = reinterpret_cast<const struct sockaddr_in6*>(&this->addr);

			if(inet_ntop(AF_INET6, &sin6->sin6_addr, res, MAXLINE) != NULL) {
					return(std::string{res});
			}

			break;
		} default: {
			printf("%s: unknown sa_family\n", __func__);
			return(std::string{});
		}
	}
}


TcpConnection::~TcpConnection() {
	shutdown(connectionFd, SHUT_RDWR);
	close(connectionFd);
	std::string log{"connection "};

	log += this->sock_ntop() + " closed";
}





IConnection::Status TcpConnection::recv(std::string& msg, size_t nbytes, int flag) const {
	ssize_t nrecv;
	char buffer[nbytes + 1];

	nrecv = ::recv(connectionFd, buffer, nbytes, flag);

	if(nrecv < 0) {
		if(errno == EAGAIN) return(IConnection::Status::AGAIN);
		logError << std::string{"recv() failed: "} + strerror(errno);

		if(errno == EAGAIN) return(IConnection::Status::AGAIN);

		return(IConnection::Status::ERROR);
	} else if(nrecv == 0) {
		return(IConnection::Status::HUP);
	}


	msg = std::string{buffer, buffer + nrecv};

	return(IConnection::Status::SUCCESS);
}


IConnection::Status TcpConnection::send(std::string const& msg, size_t nbytes, int flag) const {
	ssize_t nsend;

	if(msg.size() > nbytes) {
		logError << std::string{"send(): max message size overflow"};
	}

	nsend = ::send(connectionFd, msg.data(), nbytes, flag);

	if(nsend < 0) {
		if(errno == EAGAIN) return(IConnection::Status::AGAIN);

		logError << std::string{"send() failed: "} + std::string{strerror(errno)};
		return(IConnection::Status::ERROR);
	} 

	return(IConnection::Status::SUCCESS);
}


IConnection::Status TcpConnection::send(Packet& packet, int flag) const {
	IConnection::Status status;

	status = sendHeader(packet.header);

	if(status != IConnection::Status::SUCCESS) {
		logError << std::string{"send(Packet&) failed: "} + strerror(errno);
		return(status);
	}

	status = sendPayload(packet.payload);

	if(status != IConnection::Status::SUCCESS) {
		logError << std::string{"send(Packet&) failed: "} + strerror(errno);
		return(status);
	}

	return(status);
}


IConnection::Status TcpConnection::recv(Packet& packet, int flag) const {
	IConnection::Status status;

	status = recvHeader(packet.header);

	if(status != IConnection::Status::SUCCESS) {
		logError << std::string{"recv(Packet&) failed: "} + strerror(errno);
		return(status);
	}

	status = recvPayload(packet.payload);

	if(status != IConnection::Status::SUCCESS) {
		logError << std::string{"recvPayload(Packet&) failed: "} + strerror(errno);
		return(status);
	}

	return(status);
}

	
IConnection::Status TcpConnection::recvHeader(std::vector<uint8_t>& header) const {
	std::string tmp;
	tmp.resize(2);

	auto status = this->recv(tmp, 2, 0);

	if(status != IConnection::Status::SUCCESS) 
		return(status);

	uint16_t headerSize = (uint16_t)*(tmp.data());
	tmp.clear();

	status = this->recv(tmp, headerSize, 0);

	if(status == IConnection::Status::SUCCESS) {
		header.resize(tmp.size());
		std::memcpy(header.data(), tmp.data(), tmp.size()); // UNSAFE
		
	} else {
		logError << std::string{"recvHeader(std::vector<uint8_t>&) failed: "} + strerror(errno);
	}

	return(status);
}


IConnection::Status TcpConnection::recvPayload(std::string& payload, bool expected) const {
	IConnection::Status status = IConnection::Status::SUCCESS;

	// recvSize(size_t& msgSize) method
	uint16_t payloadSize;
	std::string buf;
	status = this->recv(buf, 2, 0);

	if(status != IConnection::Status::SUCCESS) {
		logError << std::string{"recvPayload() failed: "} + strerror(errno);
		return(status);
	}

	std::memcpy(&payloadSize, buf.data(), sizeof(uint16_t));
	// recvSize(size_t& msgSize) method END

	if(payloadSize == 0) return(IConnection::Status::SUCCESS);

	if(expected) status = this->recv(payload, payloadSize, 0);

	if(status != IConnection::Status::SUCCESS) {
		logError << std::string{"recvPayload(std::string&) failed: "} + strerror(errno);
	}

	return(status);
}



IConnection::Status TcpConnection::sendPayload(std::string const& payload, bool needSend) const {
	std::string buf;
	auto status = IConnection::Status::SUCCESS;

	// sendSize(size_t const& msgSize) method
	uint16_t payloadSize = payload.size();
	buf.resize(sizeof(uint16_t));
	std::memcpy(buf.data(), &payloadSize, sizeof(uint16_t));
	status = this->send(buf, 2, 0);
	if(status != IConnection::Status::SUCCESS) {
		logError << std::string{"sendPayload() failed: "} + strerror(errno);
		return(status);
	}
	// sendSize(size_t const& msgSize) method END


	if(needSend) status = this->send(payload);

	if(status != IConnection::Status::SUCCESS) {
		logError << std::string{"sendPayload(std::string&) failed: "} + strerror(errno);
	}

	return(status);
}



IConnection::Status TcpConnection::sendHeader(std::vector<uint8_t>const& header) const {
	std::string tmp;
	uint16_t headerSize = header.size();

	uint16_t buf[1];
	std::memcpy(buf, &headerSize, sizeof(uint16_t));
	tmp.assign(reinterpret_cast<const char*>(&headerSize), sizeof(uint16_t));
	
	// sending size of header, so receiving side knows headersize
	auto status = this->send(tmp, sizeof(uint16_t), 0);

	if(status != IConnection::Status::SUCCESS) {
		logError << std::string{"sendHeader() failed: "} + strerror(errno);
		return(status);
	}

	tmp.resize(header.size());

	std::memcpy(tmp.data(), header.data(), header.size());

	status = this->send(tmp);

	if(status != IConnection::Status::SUCCESS) {
		logError << std::string{"sendHeader(std::vector<uint8_t>&) failed: "} + strerror(errno);
	}

	return(status);
}












