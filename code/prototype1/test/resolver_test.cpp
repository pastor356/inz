#include "Resolver.hpp"
#include "TcpConnection.hpp"
#include "TcpSocket.hpp"
#include <gtest/gtest.h>



// Resolver test
TEST(ResolverTest, CLientChildResolverTest) {
	Resolver resolver;
	SocketParams servParams{AF_INET, SOCK_STREAM, SockRole::SERV, false};

	auto tcpServer = std::make_unique<TcpSocket>(
		resolver.resolve("localhost", "4242", servParams)
	);

	EXPECT_NE(tcpServer->getDescriptor(), -1);
}


// socalhost -- fail expected
TEST(ResolverTest, CorruptedDomainName) {
	Resolver resolver;
	SocketParams servParams{AF_INET, SOCK_STREAM, SockRole::SERV, false};

	EXPECT_THROW(
		resolver.resolve("socalhost", "4242", servParams), std::runtime_error
	);
}

