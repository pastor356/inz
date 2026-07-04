#pragma once
#include <sys/socket.h>
#include <iostream>
#include <netdb.h>
#include <string>
#include <cstdio>
#include <memory>
#include <vector>
#include <cstring>
#include <stdexcept>
#include <sys/types.h>
#include <unistd.h>

struct addrInfoDeleter {
	void operator() (struct addrinfo* ai) const {
		if(ai) freeaddrinfo(ai);
	}
};

enum class SockRole {Server, Client};

struct SockRes {
	int fd;
	struct sockaddr_storage addr;
	socklen_t addrlen;
};

using addrInfo = std::unique_ptr<struct addrinfo, addrInfoDeleter>;

addrInfo resolveAddr(std::string const& hostname, 
		std::string const& service, int socktype, int flag);

SockRes createSocket(std::string const& hostname, 
		std::string const& service, int socktype, SockRole role);


class TcpConn {
	int remoteFd;
	struct sockaddr_storage addr;
	socklen_t addrlen;

	TcpConn(int fd, const struct sockaddr_storage addr, socklen_t addrlen);
	TcpConn(const TcpConn& con) = delete;

public:
	friend class TcpAcc;
	friend class TcpClient;

	ssize_t send(std::string const& msg, int flags = 0) const;
	 
	ssize_t recv(std::string& msg, int flags = 0) const;

	// TODO -- overloading with sizes
	ssize_t send(std::string const& msg, size_t n, int flags = 0) const;
	ssize_t recv(std::string& msg, size_t n, int flags = 0) const; 

	~TcpConn(); // close()
};

