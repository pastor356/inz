#include "TcpConnection.hpp"
#include "TcpSocket.hpp"
#include "ConsoleLogger.hpp"
#include "SocketMock.hpp"
#include "ConnectionMock.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace ::testing;

class TcpSocketTest: public Test {
public:
	StrictMock<SocketMock> socket;
	StrictMock<ConnectionMock> conn;

	TcpSocketTest() {
		EXPECT_CALL(socket, accept()).
			WillOnce(Return(ByMove(std::unique_ptr<ConnectionMock>{nullptr})));

		auto conn = socket.accept();
		EXPECT_EQ(conn, std::unique_ptr<ConnectionMock>{nullptr});
	}
};

TEST_F(TcpSocketTest, MockingTest) {
	Packet packet;
	packet.header.push_back(3);
	packet.payload = "dupa";

	EXPECT_CALL(conn, send( Matcher<Packet&>(Ref(packet)), 0 )).
		WillOnce(Return(IConnection::Status::SUCCESS));
	
	EXPECT_EQ(conn.send(packet, 0), IConnection::Status::SUCCESS);
}



