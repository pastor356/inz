#include "Resolver.hpp"
#include "TcpConnection.hpp"
#include "Resolver.hpp"
#include "IQualityStrategy.hpp"
#include "Qos1.hpp"
#include "LiveMonitor.hpp"
#include "TcpSocket.hpp"
#include <gtest/gtest.h>
#include <sys/epoll.h>

#define MAXEVENTS 10

class QosTest: public ::testing::Test {
public:
	QosTest() {
		Resolver resolver;
		SocketParams params{AF_INET, SOCK_STREAM, SockRole::CLI, false};

		this->socket = 
			std::make_unique<TcpSocket>(resolver.resolve("localhost", "4244", params));

		SocketParams serv_params{AF_INET, SOCK_STREAM, SockRole::SERV, false};

		connection = std::move(
				socket->connect(resolver.resolve("localhost", "4242", serv_params)));

		this->connection = std::make_unique<LiveMonitor>(std::move(connection));
		this->connection = std::make_unique<Qos1>(std::move(connection));
	}

	std::unique_ptr<ISocket> socket;
	std::unique_ptr<IConnection> connection;
};

TEST_F(QosTest, MessageExhchangeTest) {

	EXPECT_NE(connection->getDescriptor(), -1);

	auto status = connection->send("dupa!");

	EXPECT_EQ(status, IConnection::Status::SUCCESS);

	std::string incoming_msg;
	status = connection->recv(incoming_msg);

	if(status == IConnection::Status::HUP) {
		logError << "connection closed! OMG :O";
	}

	logInfo << std::string{"<"} + incoming_msg + std::string{"> is new msg"};
	EXPECT_EQ(status, IConnection::Status::SUCCESS);

	incoming_msg.clear();
	status = connection->recv(incoming_msg);
	logInfo << std::string{"<"} + incoming_msg + std::string{"> is new msg"};

	EXPECT_EQ(status, IConnection::Status::SUCCESS);
}



TEST_F(QosTest, MessageExchangeTest_EPOLL) {
	int epoll_fd = epoll_create(1);
	struct epoll_event ev, events[MAXEVENTS];
	ev.events = EPOLLIN;
	ev.data.fd = connection->getDescriptor();
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, connection->getDescriptor(), &ev);

	EXPECT_NE(connection->getDescriptor(), -1);

	auto status = connection->send("dupa!");

	ssize_t nfds;
	while(1) {
		nfds = epoll_wait(epoll_fd, events, MAXEVENTS, -1);

		EXPECT_EQ(status, IConnection::Status::SUCCESS);

		std::string incoming_msg;
		status = connection->recv(incoming_msg);

		if(status == IConnection::Status::HUP) {
			epoll_ctl(epoll_fd, EPOLL_CTL_DEL, connection->getDescriptor(), NULL);
			break;
		}


		EXPECT_EQ(status, IConnection::Status::SUCCESS);

		incoming_msg.clear();
		status = connection->recv(incoming_msg);

		EXPECT_EQ(status, IConnection::Status::SUCCESS);
	}
}
