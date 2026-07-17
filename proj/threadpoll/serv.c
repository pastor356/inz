#include "serv.h"


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

void thread_make(int i) {
	pthread_create(&tptr[i].tid, NULL, thread_main, (void*) i);
}

void* thread_main(void* arg) {
	int threadno = (int) arg;
	int clifd;
	socklen_t cliaddrlen;

	for(;;) {
		cliaddrlen = addrlen;

		pthread_mutex_lock(&lock);
		if((clifd = accept(listenfd, NULL, NULL)) < 0) {
			perror("accept() in thread_main() failed");
		}

		pthread_mutex_unlock(&lock);

		web_child(clifd);
		tptr[threadno].ntasks++;
		close(clifd);
	}
}


void web_child(int sockfd) {
	char request[MAXLINE], reply[MAXLINE];
	ssize_t nrecv, nsend;
	int nbytes; 

	memset(reply, 65, MAXLINE);
	
	if((nrecv = recv(sockfd, request, MAXLINE, 0)) < 0) {
		perror("recv() in web_child()");
	} else if(nrecv == 0) {
		return;
	} else {
		nbytes = atoi(request);

		if(nbytes <= 0 || nbytes > MAXLINE) {
			printf("client requests %d bytes\n", nbytes);
			return;
		}

		if((nsend = send(sockfd, reply, nbytes, MSG_WAITALL)) < 0) {
			perror("send() in web_child()");
		}
	}
}



void sigint(int signo) {
	printf("\ncaught SIGINT\n");

	pr_cputime();
}

void pr_cputime(void) {
	double user, sys;
	struct rusage myusage, childusage;

	if (getrusage(RUSAGE_SELF, &myusage) < 0)
		perror("getrusage error");

	if (getrusage(RUSAGE_CHILDREN, &childusage) < 0)
		perror("getrusage error");

	user = (double) myusage.ru_utime.tv_sec +
	myusage.ru_utime.tv_usec / 1000000.0;
	user += (double) childusage.ru_utime.tv_sec +
	childusage.ru_utime.tv_usec / 1000000.0;

	sys = (double) myusage.ru_stime.tv_sec +
		myusage.ru_stime.tv_usec / 1000000.0;
	sys += (double) childusage.ru_stime.tv_sec +
	childusage.ru_stime.tv_usec / 1000000.0;
	printf("\enuser time = %g, sys time = %g\en", user, sys);

	exit(EXIT_SUCCESS);
}


int udp_listen(const char* hostname, const char* service, socklen_t *addrlen) {
	struct addrinfo *res, *ressave, hints;
	int err, sockfd;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if((err = getaddrinfo(hostname, service, &hints, &res)) < 0) {
		printf("getaddrinfo() failed: %s\n", gai_strerror(err));
		return(-1);
	}

	ressave = res;

	do {
		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if(sockfd < 0) continue;

		if(bind(sockfd, res->ai_addr, res->ai_addrlen) == 0) break;

		close(sockfd);

	} while((res = res->ai_next) != NULL);

	if(res == NULL) {
		perror("last bind() in udp_listen()");
	}

	if(addrlen) *addrlen = res->ai_addrlen;

	freeaddrinfo(ressave);

	return(sockfd); 
}




