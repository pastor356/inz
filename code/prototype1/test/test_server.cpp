#include "Resolver.hpp"
#include "TcpConnection.hpp"
#include "Resolver.hpp"
#include "IQualityStrategy.hpp"
#include "Qos1.hpp"
#include "LiveMonitor.hpp"
#include "TcpSocket.hpp"
#include <unordered_map>
#include <sys/epoll.h>

#ifndef 
#define MAX_EVENTS 64
#endif

void do_echo(IConnection const& conn);
void setnonblock(int fd);


int main() {
	Resolver resolver;
	SockParams params{AF_INET, SOCK_STREAM, SockRole::SERV, false};

	std::unique_ptr<ISocket> socket = 
		std::make_unique<TcpSocket>(resolver.resolve("localhost", "4242", params));

	std::unordered_map<int, std::unique_ptr<IConnection>> connections;

	int poll_fd = epoll_create(1);

	struct epoll_event ev, events[MAX_EVENTS];
	ev.event = EPOLLIN;
	ev.data.fd = socket->getDescriptor();

	epoll_ctl(poll_fd, EPOLL_CTL_ADD, socket->getDescriptor(), &ev);

	ssize_t nfds;
	while(1) {
		nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
		if(nfds < 0) {
			perror("epoll_wait() failed");
			continue;
		}

		for(size_t i = 0; i < nfds; ++i) {

			if(events[i].data.fd == socket->getDescriptor()) {
				auto newConn = socket->accept();

				connections.emplace(newConn.get(), newConn);

				ev.event = EPOLLIN | EPOLLET;
				ev.data.fd = newConn->getDescriptor();

				setnonblock(newConn->getDescriptor());

				epoll_ctl(epoll_fd, EPOLL_CTL_ADD, newConn->getDescriptor(), &ev);

				connections.emplace(newConn->getDescriptor(), std::move(newConn));

			} else {
				auto conn = connections.find(events[i].data.fd);

				if(conn != connections.end()) {
					do_echo(*(conn->second));
				}
			}

		} // for(size_t ...)

	} // while(1)

}


void setnonblock(int fd) {
	int flag = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}



void do_echo(IConnection const& conn) {
	std::string msg;
	conn.recv(msg);
	conn.send(msg);
}

