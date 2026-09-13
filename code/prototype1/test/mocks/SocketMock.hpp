#pragma once
#include <gmock/gmock.h>
#include "TcpConnection.hpp"
#include "TcpSocket.hpp"
#include "Resolver.hpp"

class SocketMock: public ISocket {
public:
	MOCK_METHOD(std::unique_ptr<IConnection>, connect, (addrInfo&&), (const, override));
	MOCK_METHOD(std::unique_ptr<IConnection>, connect, (struct sockaddr_storage), (const, override));
	MOCK_METHOD(std::unique_ptr<IConnection>, accept, (), (const, override));

	MOCK_METHOD(void, bind, (), (const, override));
	MOCK_METHOD(void, listen, (int), (const, override));
	MOCK_METHOD(int, getDescriptor, (), (const, override));
};



