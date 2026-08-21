#include "Resolver.hpp"
#include "TcpConnection.hpp"
#include "Resolver.hpp"
#include "IQualityStrategy.hpp"
#include "Qos1.hpp"
#include "LiveMonitor.hpp"
#include "TcpSocket.hpp"
#include <gtest/gtest.h>


TEST(DecoratorTest, InitTest) {
	Resolver resolver;
	SockParams params{AF_INET, SOCK_STREAM, SockRole::CLI, false};

	std::unique_ptr<ISocket> socket = 
		std::make_unique<TcpSocket>(resolver.resolve("localhost", "4242", params));

