#include "Resolver.hpp"


addrInfo Resolver::resolve(std::string const& hostname, 
		std::string const& service, SocketParams const& params) const {

	struct addrinfo *res, hints;
	int err;

	memset(&hints, 0, sizeof(struct addrinfo));

	if(params.sock_role == SockRole::SERV) 
		hints.ai_flags = AI_PASSIVE;

	if(params.numeric_host) 
		hints.ai_flags |= AI_NUMERICHOST;


	hints.ai_family = params.af_family;
	hints.ai_socktype = params.sock_type;

	const char* host = hostname.empty()? nullptr : hostname.c_str();
	const char* serv = service.empty() ? nullptr : service.c_str(); 

	if((err = getaddrinfo(host, 
					serv, &hints, &res)) != 0) {
		throw std::runtime_error{std::string{"getaddrinfo() failed: "} + gai_strerror(err)};
		return(addrInfo(nullptr, addrinfoDeleter));
	}

	return(addrInfo(res, addrinfoDeleter));
}


void addrinfoDeleter(struct addrinfo* ai) {
	if(ai) freeaddrinfo(ai);
	return;
}


