#pragma once
#include "IConnection.hpp"

class TcpCaller: public TcpConnection {
public:
	TcpCaller(int remoteFd, std::string const& addr, int port);
};

