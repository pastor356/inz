#include "TcpSocket.hpp"
#include "TcpConnection.hpp"

#include "Session.hpp"
#include "Handshaker.hpp"
#include "DecoratorBuilder.hpp"
#include "MonitoredNode.hpp"
#include "InitialState.hpp"
#include "Serializer.hpp"
#include "SessionClient.hpp"

#include "MessageRouter.hpp"
#include "AcceptorSocket.hpp"
#include "NodeInitializer.hpp"

#include "Reactor.hpp"
#include "Demultiplexer.hpp"
#include "Invoker.hpp"

#include <signal.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	enableLogger();
	configureLogger("CLIENT");

	if(argc < 6) {
		std::cout << "client usage: ./client topic login password message #messages" << std::endl;
		exit(1);
	}

	Resolver resolver;
	std::unique_ptr<ISocket> socket = std::make_unique<TcpSocket>(
			resolver.resolve("127.0.0.1", "4243", 
				SocketParams{AF_INET, SOCK_STREAM, SockRole::CLI, true}));


	auto conn = socket->connect(resolver.resolve("127.0.0.1", "4242", 
				SocketParams{AF_INET, SOCK_STREAM, SockRole::CLI, true}));



	Packet packet = Serializer::serializeInitial();

	conn->send(packet);

	packet = Serializer::serializeHandshake(
		ROLE_PROD,
		1,
		true,
		true,
		RIGHTS_USR,
		argv[1], // topic
		argv[2], // login
		argv[3], // password
		"secret");
	
	conn->send(packet); // Handshake
	Packet incoming;
	auto status = conn->recv(incoming); // handshake ack

	conn = std::make_unique<LiveMonitor>(std::move(conn));
	conn = std::make_unique<Qos1>(std::move(conn));

	auto sess = std::make_unique<SessionClient>(std::move(conn));


	incoming.header.clear();
	incoming.payload.clear();

	size_t nmsg = atoi(argv[5]);

	for(size_t i = 0; i < nmsg; ++i) {
		Packet data = Serializer::serializeData(argv[1]);
		data.payload = argv[4];

		data.payload += std::to_string(i);

		sess->send(data);
		std::cout << "SENT MESSAGE No: " << i << std::endl;
		sess->recv(incoming, MSG_DONTWAIT);
	}

	pause();

	return(0);
}

