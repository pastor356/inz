#pragma once
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <sys/resource.h>

#define MAXLINE 256
#define QLEN 10

extern pthread_mutex_t lock;

typedef struct {
	pthread_t tid;
	size_t ntasks;
} Thread;

extern Thread* tptr;
extern int listenfd, nthreads;
extern socklen_t addrlen;


int tcp_listen(const char* hostname, const char* service, socklen_t* addrlen);
int udp_listen(const char* hostname, const char* service, socklen_t *addrlen);

void thread_make(int i);
void* thread_main(void* arg);

void web_child(int sockfd);
void sigint(int signo);
void pr_cputime(void);
