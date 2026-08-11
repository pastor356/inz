#pragma once
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <netdb.h>
#include <cerrno>
#include <cstring>
#include <arpa/inet.h>


constexpr size_t MAXMSGSIZ = 1024;
constexpr size_t MAXLINE = MAXMSGSIZ;

class IConnection {
public:
	enum class Status: ssize_t {
		SUCCESS = 0,
		HUP, 
		ERROR
	};

	virtual void send(std::string const& msg, int flag = 0) const = 0;
	virtual Status recv(std::string& msg, int flag = 0) const = 0;
	virtual const struct sockaddr_storage getAddr() const = 0;
};


