#include "TcpConnection.hpp"
#include "TcpSocket.hpp"
#include "ConsoleLogger.hpp"
#include "ConnectorMock.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cstring>

using namespace ::testing;


TEST(NetworkTest, PARENT_CHILD_EXCHANGE) {
	std::cerr << std::endl << std::endl;
	auto tcpSocketServer = std::make_unique<TcpSocket>("127.0.0.1", 4242);
	tcpSocketServer->bind();
	tcpSocketServer->listen(5);


	pid_t client;
	if((client = fork()) < 0) {
		logError << "fork() failed: " << strerror(errno);
	} else if(client == 0) {

		auto tcpSocketClient = std::make_unique<TcpSocket>("127.0.0.1", 4243);
		struct sockaddr_in a;
		std::memset(&a, 0, sizeof(struct sockaddr_in));
		a.sin_family = AF_INET;
		a.sin_port = htons(4242);
		inet_pton(AF_INET, "127.0.0.1", &a.sin_addr);

		auto* addr = reinterpret_cast<struct sockaddr_storage*>(&a);

		sleep(1);

		auto tcpConnectionToServer = (*tcpSocketClient).connect(*addr);

		std::string msgChild{"blalba!"};
		(*tcpConnectionToServer).send(msgChild);

	} else {
		std::string msg;

		auto tcpConnectionToClient = (*tcpSocketServer).accept();
		IConnection::Status status = (*tcpConnectionToClient).recv(msg);
		
		logInfo << msg;
		EXPECT_EQ(msg, std::string{"blalba!"});
		EXPECT_EQ(msg.size(), strlen("blalba!"));

		wait(NULL);
		if(status == IConnection::Status::SUCCESS) {
			logInfo << "SUCCESS";
		} else {
			logError << "FAILURE";
		}
	}
}















class TcpSocketTest: public Test {
public:
	StrictMock<SocketMock> socket;
};


TEST_F(TcpSocketTest, MockingTest) {
	EXPECT_CALL(socket, accept()).WillOnce(Return(ByMove(std::unique_ptr<IConnection>{})));
	socket.accept();
}


