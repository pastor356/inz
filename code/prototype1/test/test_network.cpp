#include "TcpConnection.hpp"
#include "TcpSocket.hpp"
#include "ConsoleLogger.hpp"
#include <gtest/gtest.h>



TEST(dupa, dupa) {
	auto tcpSocketServer = std::make_unique<TcpSocket>("127.0.0.1", 4242);
	(*tcpSocketServer).bind();
	(*tcpSocketServer).listen(5);

	auto tcpSocketClient = std::make_unique<TcpSocket>("127.0.0.1", 4243);

	pid_t client;
	if((client = fork()) < 0) {
		logError << "fork() failed: " << strerror(errno);
	} else if(client == 0) {
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

		wait(NULL);
		if(status == IConnection::Status::SUCCESS) {
			logInfo << "SUCCESS";
		} else {
			logError << "FAILURE";
		}

	}
}




