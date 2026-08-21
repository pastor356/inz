#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <memory>
#include <string> 
#include <cstring>

#include "ConsoleLogger.hpp"

using addrInfo = std::unique_ptr<struct addrinfo, void(*)(struct addrinfo*)>;

void addrinfoDeleter(struct addrinfo*);

enum class SockRole {
	SERV,
	CLI
};

struct SocketParams {
	sa_family_t af_family;
	int sock_type;
	SockRole sock_role;
	bool numeric_host;

	SocketParams(sa_family_t af, int st, SockRole sr, bool nh):
		af_family(af), sock_type(st), sock_role(sr), numeric_host(nh) {}
};


class Resolver {
public:
	addrInfo resolve(std::string const& hostname, 
			std::string const& service, SocketParams const& params) const; 
};



