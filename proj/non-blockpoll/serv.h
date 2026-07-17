#pragma once
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>

#define QLEN 50
#define MAXEVENTS 256
#define MAXLINE 256

extern int pollfd;

void setnonblock(int fd);
int tcp_listen(const char* hostname, const char* service, socklen_t* addrlen);
void handle_request_nonblock(int connfd);
void sigint(int signo);
