#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define MAXLINE 256
int tcp_connect(const char* hostname, const char* service); 
int udp_connect(const char* hostname, const char* service); 

int main(int argc, char** argv) {
	if(argc != 6) {
		printf("usage: ./client hostname service #children #loops #bytes\n");
		exit(EXIT_FAILURE);
	}

	pid_t pid; 

	size_t nloops = atoi(argv[4]);
	size_t nchildren = atoi(argv[3]);
	size_t nbytes = atoi(argv[5]);

	for(size_t i = 0; i < nchildren; ++i) {

		if((pid = fork()) == 0) {
			char request[MAXLINE];
			char reply[MAXLINE];
			int sockfd = tcp_connect(argv[1], argv[2]);

			int n = snprintf(request, MAXLINE, "%d\n", nbytes);

			for(size_t j = 0; j < nloops; ++j) {
				if((n = send(sockfd, request, n, 0)) < 0) {
					perror("send() failed");
				}

				if((n = recv(sockfd, reply, nbytes, 0)) < 0) {
					perror("recv() failed");
				}

				printf("<%s> is reply\n<%d> is lenth\n", reply, n);
			}

			close(sockfd);
			printf("child %d is done\n\n", i);
			exit(EXIT_SUCCESS);
		}
	}

	while(wait(NULL) > 0);

	exit(EXIT_SUCCESS);
}


int tcp_connect(const char* hostname, const char* service) {
	int sockfd, err;
	struct addrinfo *res, *ressave, hints;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if((err = getaddrinfo(hostname, service, &hints, &res)) != 0) {
		printf("getaddrinfo() failed: %s\n", gai_strerror(err));
		return(-1);
	}

	ressave = res;

	do {
		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if(sockfd < 0) continue;

		if(connect(sockfd, res->ai_addr, res->ai_addrlen) == 0) break;
	} while((res = res->ai_next) != NULL);

	if(res == NULL) {
		perror("last connect() failure");
		return(-1);
	}

	freeaddrinfo(ressave);

	return(sockfd);
}


int udp_connect(const char* hostname, const char* service) {
	int sockfd, err;
	struct addrinfo *res, *ressave, hints;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if((err = getaddrinfo(hostname, service, &hints, &res)) != 0) {
		printf("getaddrinfo() failed: %s\n", gai_strerror(err));
		return(-1);
	}

	ressave = res;

	do {
		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if(sockfd < 0) continue;

		if(connect(sockfd, res->ai_addr, res->ai_addrlen) == 0) break;
	} while((res = res->ai_next) != NULL);

	if(res == NULL) {
		perror("last connect() failure");
		return(-1);
	}

	freeaddrinfo(ressave);

	return(sockfd);
}
