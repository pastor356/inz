#include "TcpSocket.hpp"
#include "TcpConnection.hpp"

#include "Session.hpp"
#include "Handshaker.hpp"
#include "DecoratorBuilder.hpp"
#include "MonitoredNode.hpp"
#include "InitialState.hpp"
#include "Serializer.hpp"

#include "MessageRouter.hpp"
#include "AcceptorSocket.hpp"
#include "NodeInitializer.hpp"

#include "Reactor.hpp"
#include "Demultiplexer.hpp"
#include "Invoker.hpp"

#include <signal.h>

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
		ROLE_CONS,
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
	auto status = conn->recv(incoming);
	status = conn->recv(incoming);

	uint16_t sessionId;
	memcpy(&sessionId, incoming.payload.data(), sizeof(sessionId));

	std::cout << "SESSION ID: " << sessionId << std::endl;

	conn = std::make_unique<LiveMonitor>(std::move(conn));
	conn = std::make_unique<Qos1>(std::move(conn));

	Packet data = Serializer::serializeData(argv[1]);

	data.payload = std::string{argv[4]};

	incoming.header.clear();
	incoming.payload.clear();
	for(size_t i = 0; i < atoi(argv[5]); ++i) {
		conn->send(data);

		conn->recv(incoming, MSG_DONTWAIT);
	}


	return(0);
}

