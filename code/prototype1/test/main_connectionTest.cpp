#include "TcpConnection.hpp"
#include "TcpSocket.hpp"
#include "ConsoleLogger.hpp"
#include <sys/wait.h>

#define LISTEN_BACKLOG 5


int main(void) {

	auto tcpSocketChild = std::make_shared<TcpSocket>("127.0.0.1", 4243);

	struct sockaddr_in a;
	std::memset(&a, 0, sizeof(struct sockaddr_in));
	a.sin_family = AF_INET;
	a.sin_port = htons(4242);
	inet_pton(AF_INET, "127.0.0.1", &a.sin_addr);

	auto* addr = reinterpret_cast<struct sockaddr_storage*>(&a);

	sleep(1);

	auto tcpConnectionChild = (*tcpSocketChild).connect(*addr);

	std::string msgChild{"blalba!"};
	(*tcpConnectionChild).send(msgChild);

	return(0);

}



