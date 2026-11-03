#pragma once
#include "IConnection.hpp"

class SessionClient {
	std::unique_ptr<IConnection> conn;
	uint16_t sessionId;





