#pragma once
#include "TcpConnection.hpp"

class TcpCaller: public TcpConnection {
public:
	TcpCaller(int remoteFd, const struct sockaddr_storage addr);
};

