#include "AppendRequest.hpp"
#include "DatabaseAccessor.hpp"
#include "Scheduler.hpp"
#include "Qos1.hpp"
#include "IConnection.hpp"
#include <signal.h>
#include "Resolver.hpp"
#include "TimerQueue.hpp"
#include "TcpSocket.hpp"
#include "TcpConnection.hpp"
#include "Serializer.hpp"
#include "SessionClient.hpp"

volatile sig_atomic_t quitflag = 0;
void sigint(int signo);

char appname[64];

int main(int argc, char* argv[]) {
	(void) argc;
	memcpy(appname, argv[0], strlen(argv[0]));

	sigset_t mask;
	sigfillset(&mask);
	sigdelset(&mask, SIGINT);
	sigprocmask(SIG_SETMASK, &mask, NULL);

	struct sigaction act;
	act.sa_handler = sigint;
	act.sa_flags = 0; 
	sigemptyset(&act.sa_mask);
	sigaction(SIGINT, &act, NULL);


	enableLogger();
	
	auto db = std::make_shared<DatabaseAccessor>(
				 std::string{"http://localhost:8086?db=consAppDB"});


	if(db.get() == nullptr) {
		 logError << std::string{"Database access failed"};
	} else {
		logInfo << std::string{"Database connection established"};
	}


	Resolver resolver;
	std::unique_ptr<ISocket> socket = std::make_unique<TcpSocket>(
			resolver.resolve("127.0.0.1", "4244", 
				SocketParams{AF_INET, SOCK_STREAM, SockRole::CLI, true}));


	auto conn = socket->connect(resolver.resolve("127.0.0.1", "4242", 
				SocketParams{AF_INET, SOCK_STREAM, SockRole::CLI, true}));


	auto timer = std::make_shared<TimerQueue>();

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
	auto status = conn->recv(incoming); // handshake ack

	conn = std::make_unique<Qos1>(std::move(conn), timer);

	auto sess = std::make_unique<SessionClient>(std::move(conn));


	while(quitflag != 1)  {




	}


	return(0);
}

void sigint(int signo) {
	if(signo == SIGINT) {
		logInfo << std::string{appname} + " quits";
		quitflag = 1;
	}
}

