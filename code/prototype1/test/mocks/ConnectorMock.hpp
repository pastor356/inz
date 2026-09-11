#include <gmock/gmock.h>
#include "TcpConnection.hpp"

class ConnectionMock: public Connector {
	MOCK_CONST_METHOD(operator(), int(int, struct sockaddr*, socklen_t));
};
