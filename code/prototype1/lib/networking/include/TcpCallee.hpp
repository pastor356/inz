#pragma once
#include "TcpConnection.hpp"

class TcpCallee: public TcpConnection {
public:
	TcpCallee(int sockFd, const struct sockaddr_storage addr);
};
