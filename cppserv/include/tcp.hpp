#pragma once
#include <sys/socket.h>
#include <cstdio>
#include <netdb.h>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <cstring>
#include <stdexcept>
#include <sys/types.h>
#include <unistd.h>

#include "network.hpp"


extern int errno;

class TcpClient {
	std::string hostname, service;

public:
	TcpClient() = delete;
	TcpClient(std::string const& hostname, std::string const& service);

	std::unique_ptr<TcpConn> connect() const;
};


class TcpAcc {
	int servFd;
	std::string host, service;
	struct sockaddr_storage selfAddr;
	socklen_t addrlen;

public:
	TcpAcc(std::string const& hostname, std::string const& service);
	TcpAcc(const TcpAcc& acc) = delete;

	int listen(int backlog) const;
	std::unique_ptr<TcpConn> accept() const; // fills sockaddr_storage in TcpConn
	~TcpAcc(); // close()
};




class ServTcp {
	std::unique_ptr<TcpAcc> serv;

	using connVec = std::vector<std::unique_ptr<TcpConn>>;
	connVec connections;

public:
	ServTcp(std::unique_ptr<TcpAcc> &&serv);
	int pushConnBack(TcpAcc const& newConn); // copies new connection into vector
	void event_loop() const;
};

