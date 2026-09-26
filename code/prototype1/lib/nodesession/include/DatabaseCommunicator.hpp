#pragma once
#include "TcpConnection.hpp"



class DatabaseCommunicator {
	std::shared_ptr<IConnection> conn;
};

