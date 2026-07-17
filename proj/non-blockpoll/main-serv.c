#include "serv.h"


int main(int argc, char** argv) {
	if(argc != 3) {
		printf("usage: ./serv hostname service\n");
		exit(EXIT_SUCCESS);
	}

	int sockfd, flags, err;
	struct epoll_event ev;

	sockfd = tcp_listen(argv[1], argv[2], NULL);
	if(sockfd < 0) perror("tcp_listen() failed");

	setnonblock(sockfd);

	pollfd = epoll_create(1);
	if(pollfd < 0) printf("epoll_create() failed\n");
	
	ev.events = EPOLLIN;
	ev.data.fd = sockfd;
	err = epoll_ctl(pollfd, EPOLL_CTL_ADD, sockfd, &ev);
	
	
	int nfds, clifd;
	size_t i;
	for(;;) {
		struct epoll_event events[MAXEVENTS];
		nfds = epoll_wait(pollfd, events, MAXEVENTS, -1);

		if(nfds < 0) {
			perror("epoll_wait() failed");
			continue;
		}

		struct sigaction act;
		act.sa_flags = 0;
		act.sa_handler = sigint;
		sigemptyset(&act.sa_mask);
		sigaction(SIGINT, &act, NULL);

		for(i = 0; i < nfds; ++i) {

			if(events[i].events & (EPOLLIN | EPOLLHUP)) {

				if(events[i].data.fd == sockfd) {
					clifd = accept(sockfd, NULL, NULL);
					if(clifd < 0) {
						perror("accept() failed");
						continue;
					}

					setnonblock(clifd);
					ev.events = EPOLLIN | EPOLLET;
					ev.data.fd = clifd;
					if(epoll_ctl(pollfd, EPOLL_CTL_ADD, clifd, &ev) < 0) 
						perror("epoll_ctl() failed");

				} else {
					handle_request_nonblock(events[i].data.fd);
				}
			}
		}
	}
}















			

		






