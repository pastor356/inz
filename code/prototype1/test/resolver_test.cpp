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

// numeric_host == true -- fail expected
TEST(ResolverTest, NumericHostFailure) {
	Resolver resolver;
	SocketParams servParams{AF_INET, SOCK_STREAM, SockRole::SERV, true};

	EXPECT_THROW(
		resolver.resolve("localhost", "4242", servParams), std::runtime_error
	);
}

TEST(ResolverTest, NonNumericHostFailure) {
	Resolver resolver;
	SocketParams servParams{AF_INET, SOCK_STREAM, SockRole::SERV, true};

	EXPECT_NO_THROW(
		resolver.resolve("127.0.0.1", "4242", servParams)
	);
}
