#include "TcpConnection.hpp"
#include "TcpSocket.hpp"
#include "ConsoleLogger.hpp"
#include <sys/wait.h>

#define LISTEN_BACKLOG 5

int main() {
	auto tcpSocketParent = std::make_shared<TcpSocket>("127.0.0.1", 4242);

	(*tcpSocketParent).bind();
	(*tcpSocketParent).listen(LISTEN_BACKLOG);

	auto tcpConnectionParent = (*tcpSocketParent).accept();

	std::string msg;
	IConnection::Status status = (*tcpConnectionParent).recv(msg);

	std::cout << msg << std::endl;

	if(status == IConnection::Status::SUCCESS) {
		return(0);
	} else {
		return(1);
	}
}
