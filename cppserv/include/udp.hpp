#pragma once
#include "network.hpp"

class UdpPeer {
	struct sockaddr_storage remoteAddr;
	socklen_t addrlen;

public:
	UdpPeer();
	UdpPeer(struct sockaddr_storage &addr, socklen_t addrlen);
	UdpPeer(std::string const& hostname, std::string const& service, SockRole role);

	void setAddr(struct sockaddr_storage &addr, socklen_t addrlen);
	void setAddr(std::string const& addr);

	ssize_t send(int servFd, std::string const& msg, size_t n, int flags) const;

	// uses connect() to receive msg from particular address
	// connect(AF_UNSPEC) after recv()
	ssize_t recv(int servFd, std::string &msg, size_t n, int flags) const; 

	explicit operator struct sockaddr() const;

	~UdpPeer(); 
};


class ServUdp {
	int servFd;
	struct sockaddr_storage servAddr;
	socklen_t addrlen; 

	using udpPeers = std::vector<std::unique_ptr<UdpPeer>>;
	udpPeers peers;
	ServUdp(ServUdp const& serv) = delete;
	ServUdp& operator = (ServUdp& serv) = delete; 

public:
	ServUdp(std::string const& hostname, std::string const& service);

	// TODO: overload with `size_t n` and `UdpPeer const&` params
	ssize_t sendto(std::string const& msg, struct sockaddr_storage const& addr, int flags) const;
	ssize_t recvfrom(std::string& msg, struct sockaddr_storage& addr, int flag) const;
	
	ssize_t sendto(std::string const& msg, UdpPeer const& peer, int flags) const;
	ssize_t recvfrom(std::string& msg, UdpPeer& peer, int flags);
	
	void handle_request() const; 
	~ServUdp(); // close()
};

