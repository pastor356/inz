#include "TcpSocket.hpp"
#include "TcpConnection.hpp"

#include "Session.hpp"
#include "Handshaker.hpp"
#include "DecoratorBuilder.hpp"
#include "MonitoredNode.hpp"
#include "InitialState.hpp"
#include "Serializer.hpp"

#include "MessageRouter.hpp"
#include "EchoRouter.hpp"
#include "AcceptorSocket.hpp"
#include "NodeInitializer.hpp"

#include "Reactor.hpp"
#include "Demultiplexer.hpp"
#include "Invoker.hpp"

#include <signal.h>
void sigint(int signo);
std::shared_ptr<Reactor> reactor;

int main() {
	enableLogger();
	configureLogger("BROKER");

	sigset_t sigset;
	sigfillset(&sigset);
	sigdelset(&sigset, SIGINT);

	if(sigprocmask(SIG_BLOCK, &sigset, nullptr) < 0) {
		perror("sigprocmask() failed");
	}

	struct sigaction act;
	act.sa_handler = sigint;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);

	sigaction(SIGINT, &act, nullptr);
	


	Resolver resolver;
	std::unique_ptr<ISocket> tcpSocket = std::make_unique<TcpSocket>(resolver.resolve(
				"127.0.0.1", "4242", 
				SocketParams{AF_INET, SOCK_STREAM, SockRole::SERV, true}));

	tcpSocket->bind();
	tcpSocket->listen(5);

	std::shared_ptr<IDataAccessor> dataAccessor = std::make_shared<CacheAccessor>();
	std::shared_ptr<NodeRegistry> nodeRegistry = std::make_shared<NodeRegistry>();

	std::shared_ptr<IPoll> poller = 
		std::make_shared<Demultiplexer>(std::make_unique<EPollWaiter>());

	std::shared_ptr<IInvoker> dispatcher = std::make_shared<Invoker>();
	reactor = std::make_shared<Reactor>(poller, dispatcher);

	std::shared_ptr<EventHandler> messageRouter = std::make_shared<EchoRouter>(reactor);
	std::shared_ptr<EventHandler> nodeInitializer = std::make_shared<NodeInitializer>(reactor,
		messageRouter);

	std::shared_ptr<IStateContext> acceptorNode = std::make_shared<AcceptorNode>(
		std::move(tcpSocket));

	std::shared_ptr<EventHandler> acceptorSocket = std::make_shared<AcceptorSocket>(
		 reactor, dataAccessor, nodeRegistry, nodeInitializer);

	reactor->registerNode(acceptorNode, acceptorSocket);

	reactor->run();

	return(0);
};


void sigint(int signo) {
	reactor->quit();
}

