#include "TcpConnection.hpp"
#include "Resolver.hpp"
#include "IQualityStrategy.hpp"
#include "Qos1.hpp"
#include "LiveMonitor.hpp"
#include "TcpSocket.hpp"
#include <gtest/gtest.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <poll.h>

#define MAXEVENTS 10

class QosTest: public ::testing::Test {
public:
	QosTest() { 
		Resolver resolver; 
		SocketParams params{AF_INET, SOCK_STREAM, SockRole::CLI, false};

		this->socket = std::make_unique<TcpSocket>(
				resolver.resolve("localhost", "4244", params));

		SocketParams serv_params{AF_INET, SOCK_STREAM, SockRole::SERV, false};

		connection = std::move(
				socket->connect(resolver.resolve("localhost", "4242", serv_params)));

	}

	std::unique_ptr<ISocket> socket;
	std::unique_ptr<IConnection> connection;
};


TEST_F(QosTest, MessageExhchangeTest) {

	this->connection = std::make_unique<LiveMonitor>(std::move(this->connection));
	this->connection = std::make_unique<Qos1>(std::move(this->connection));

	int flag = fcntl(connection->getDescriptor(), F_GETFL, 0);
	fcntl(connection->getDescriptor(),  F_SETFL, flag | O_NONBLOCK);

	EXPECT_NE(connection->getDescriptor(), -1);

	auto status = connection->send("dupa!");

	EXPECT_EQ(status, IConnection::Status::SUCCESS);

	std::string incoming_msg;
	struct pollfd polls[1];
	std::memset(polls, 0, sizeof(struct pollfd));

	polls[0].fd = connection->getDescriptor();
	polls[0].events = EPOLLIN;

	poll(polls, 1, -1);
	while(status != IConnection::Status::AGAIN) {
		status = connection->recv(incoming_msg);

		if(status == IConnection::Status::SUCCESS) logInfo << "success";
		else if(status == IConnection::Status::AGAIN) logInfo << "eagain";
		else logInfo << "error";
	} 

	if(status == IConnection::Status::HUP) {
		logError << "connection closed! OMG :O";
	}

	logInfo << std::string{"<"} + incoming_msg + std::string{"> is new msg"};
	EXPECT_EQ(status, IConnection::Status::AGAIN);
	logWarn << std::string{"\033[31m<"} + incoming_msg + std::string{">\033[0m is new msg"};

}


TEST_F(QosTest, KeepAlivePacketTest) {
	Packet packet;
	this->connection = std::make_unique<LiveMonitor>(std::move(connection));
	packet.header.push_back(MSGTYPE_KEEPALIVE);
	connection->send(packet);
}
