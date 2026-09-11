#include "TcpCaller.hpp"

TcpCaller::TcpCaller(int sockFd, const struct sockaddr_storage addr): 
	TcpConnection(sockFd, addr) {}

