#pragma once
#include "TcpConnection.hpp"
#include "TcpSocket.hpp"
#include "ConsoleLogger.hpp"
#include "SocketMock.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>


class ConnectionMock: public IConnection {
public:
	MOCK_METHOD(IConnection::Status, send, (Packet&, int), (const));
	MOCK_METHOD(IConnection::Status, recv, (Packet&, int), (const));

	MOCK_METHOD(IConnection::Status, send, (std::string const&, int), (const));
	MOCK_METHOD(IConnection::Status, recv, (std::string&, int), (const));

	MOCK_METHOD(IConnection::Status, sendHeader, (std::vector<uint8_t> const&), (const));
	MOCK_METHOD(IConnection::Status, recvHeader, (std::vector<uint8_t>&), (const));
	MOCK_METHOD(IConnection::Status, sendPayload, (std::string const&), (const));
	MOCK_METHOD(IConnection::Status, recvPayload, (std::string&), (const));

	MOCK_METHOD(const struct sockaddr_storage, getAddr, (), (const));
	MOCK_METHOD(int, getDescriptor, (), (const));
};

