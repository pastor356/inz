#include "serv.h"

int pollfd;

int tcp_listen(const char* hostname, const char* service, socklen_t* addrlen) {
	struct addrinfo *res, *ressave, hints;
	int listenfd, err, reuse = 1;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if((err = getaddrinfo(hostname, service, &hints, &res)) < 0) {
		printf("getaddrinfo() failed: %s\n", gai_strerror(err));
		return(-1);
	}

	ressave = res;

	do {
		listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if(listenfd < 0) continue;

		if(bind(listenfd, res->ai_addr, res->ai_addrlen) == 0) break;

	} while((res = res->ai_next) != NULL);

	if(res == NULL) {
		perror("last bind()");
		return(-1);
	}

	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));

	if(listen(listenfd, QLEN) < 0) perror("listen() failed");

	if(addrlen) *addrlen = res->ai_addrlen;
	freeaddrinfo(ressave);

	return(listenfd);
}


void setnonblock(int fd) {
	int flag = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flag | O_NONBLOCK);

	return;
}

void handle_request_nonblock(int connfd) {
	char request[MAXLINE];
	char reply[MAXLINE];
	ssize_t nrecv = 0;
	int err = 0;
	int nbytes;


	while(1) {
		err = recv(connfd, request+nrecv, 8, 0);
		if(err < 0 && errno == EWOULDBLOCK) {
			break; 
		} else if(err == 0) {
			printf("received o bytes => connection closed\n");
			epoll_ctl(pollfd, EPOLL_CTL_DEL, connfd, NULL);
			break;
		} else nrecv += err;

		if(nrecv == MAXLINE) {
			printf("%s: message too long\n");
			return;
		}
	}

	nbytes = atoi(request);		
	memset(reply, 65, nbytes);

	err = send(connfd, reply, nbytes, 0);
	printf("<%s> is reply\n<%d> is length of reply\n", reply, err);
	if(err < 0) perror("send() failed");
}

void sigint(int signo) { 
	printf("\ncaught SIGINT\n");
	exit(EXIT_SUCCESS);
}
