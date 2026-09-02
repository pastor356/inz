#include "Resolver.hpp"
#include "TcpConnection.hpp"
#include "Resolver.hpp"
#include "IQualityStrategy.hpp"
#include "Qos1.hpp"
#include "LiveMonitor.hpp"
#include "TcpSocket.hpp"
#include <unordered_map>
#include <sys/epoll.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#ifndef MAX_EVENTS 
#define MAX_EVENTS 64
#endif

constexpr int LISTEN_BACKLOG = 5;

void do_echo(IConnection const& conn);
void setnonblock(int fd);

volatile sig_atomic_t quitflag = 0;
void set_sigact(int signo, void(*handler)(int));
void sigint(int signo);



int main() {
	enableLogger();
	configureLogger("Test Server");

	sigset_t currset;
	sigfillset(&currset);
	sigdelset(&currset, SIGINT);
	sigprocmask(SIG_BLOCK, &currset, NULL);
	set_sigact(SIGINT, sigint);



	Resolver resolver;
	SocketParams params{AF_INET, SOCK_STREAM, SockRole::SERV, false};

	std::unique_ptr<ISocket> socket = 
		std::make_unique<TcpSocket>(resolver.resolve("localhost", "4242", params));

	socket->bind();

	socket->listen(LISTEN_BACKLOG);

	std::unordered_map<int, std::unique_ptr<IConnection>> fdsToConnections;

	int poll_fd = epoll_create(1);

	struct epoll_event ev, events[MAX_EVENTS];
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = socket->getDescriptor();
	epoll_ctl(poll_fd, EPOLL_CTL_ADD, socket->getDescriptor(), &ev);

	ssize_t nfds = 0;
	while(quitflag != 1) {
		nfds = epoll_wait(poll_fd, events, MAX_EVENTS, -1);
		if(nfds < 0) {
			perror("epoll_wait() failed");
			continue;
		}

		for(size_t i = 0; i < nfds; ++i) {

			if(events[i].data.fd == socket->getDescriptor()) {
				auto newConn = socket->accept();

				newConn = std::make_unique<LiveMonitor>(std::move(newConn));
				newConn = std::make_unique<Qos1>(std::move(newConn));

				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = newConn->getDescriptor();

				setnonblock(newConn->getDescriptor());

				epoll_ctl(poll_fd, EPOLL_CTL_ADD, newConn->getDescriptor(), &ev);

				fdsToConnections.emplace(newConn->getDescriptor(), std::move(newConn));

			} else {
				auto conn = fdsToConnections.find(events[i].data.fd);

				if(conn != fdsToConnections.end()) {
					do_echo(*(conn->second));
				}
			}

		} // for(size_t ...)

		nfds = 0;

	} // while(1)


	return(0);
}



void setnonblock(int fd) {
	int flag = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}


void do_echo(IConnection const& conn) {
	std::string msg;
	auto status = conn.recv(msg);
	if(status == IConnection::Status::SUCCESS) {
		conn.send(msg);
		logInfo << std::string{"<"} + msg + std::string{"> is new msg"};
	}
}



void set_sigact(int signo, void(*handler)(int)) {
	struct sigaction act;
	act.sa_handler = handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(signo, &act, NULL);
}


void sigint(int signo) {
	quitflag = 1;
}



