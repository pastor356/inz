#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <stddef.h>
#include <sys/un.h>
#include <signal.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <pthread.h>
#include <poll.h>
#include <errno.h>
#include <stdint.h>

#include "fdarr.h"

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#define SERVSOCK "./serv.sock"
#define VBUFSIZ 64
#define QLEN 5
#define DELIM "|"

extern volatile sig_atomic_t quitflag;

int set_sigact(int signo, void (*sig_h)(int), sigset_t* set);
void sigint(int signo);

int init_client(const char* path, const char* servpath, struct sockaddr_un* sun, ssize_t* slen);
int init_serv(const char* path);
void* thr_func(void* arg);
void print_help(void);



typedef struct topic_info {
	fdarr_t* fdarr;
	char* topic_name;
	pthread_rwlock_t m;
	fdarr_t* prod_fdarr;
	pthread_t* tid;
	int notif[2];
} tinfo_t;

tinfo_t* create_tinfo(void);
int init_tinfo(tinfo_t* topic, const char* topicname);
int destroy_tinfo(tinfo_t* topic);
int set_topicname_tinfo(tinfo_t* topic, const char* topicname);

ssize_t recvn(int fd, void* ptr, size_t n, int flag);
ssize_t sendn(int fd, void* ptr, size_t n, int flag);

int get_msg(char* buf, size_t sz);
int gettopicinfo(const char* serv, const char* topic, uint8_t flags);
