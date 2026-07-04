#include "header.h"

volatile sig_atomic_t quitflag = 0;

int set_sigact(int signo, void(*sig_h)(int), sigset_t* set) {
	struct sigaction act;
	sigset_t blockset;
	sigfillset(&blockset);
	sigdelset(set, signo);

	if(sigprocmask(SIG_SETMASK, set, NULL) != 0) {
		perror("sigprocmask() in set_sigact()");
		return(-1);
	}

	act.sa_handler = sig_h;
	act.sa_mask = blockset;
	act.sa_flags = 0;

	if(sigaction(signo, &act, NULL) != 0) {
		perror("sigaction() in set_sigact()");
		return(-2);
	}

	return(0);
}


void sigint(int signo) {
	if(signo == SIGINT) quitflag = 1;
}
	

tinfo_t* create_tinfo(void) {
	tinfo_t* newtopic = (tinfo_t*) malloc(sizeof(tinfo_t));
	if(!newtopic) {
		printf("%s: malloc() failed\n", __func__);
		return(NULL);
	}

	return(newtopic);
}



int init_tinfo(tinfo_t* topic, const char* topicname) {
	if(!topic) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	if(init_fdarr(&topic->fdarr, 10) < 0) {
		printf("init_tinfo(): init_arr() failed\n");
		return(-1);
	}

	if(init_fdarr(&topic->prod_fdarr, 10) < 0) {
		printf("init_tinfo(): init_arr() failed\n");
		destroy_fdarr(topic->fdarr);
		return(-1);
	}
	
	topic->topic_name = (char*) malloc(VBUFSIZ);
	if(!topic->topic_name) {
		printf("%s: malloc() failed\n", __func__);
		destroy_fdarr(topic->fdarr);
		destroy_fdarr(topic->prod_fdarr);
		return(-1);
	}

	pthread_rwlock_init(&topic->m, NULL);
	topic->tid = (pthread_t*) malloc(sizeof(pthread_t));
	strcpy(topic->topic_name, topicname);

	return(0);
}


int set_topicname_tinfo(tinfo_t* topic, const char* topicname) { 
	if(!topic && !topicname) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	if(topic->topic_name) 
		strcpy(topic->topic_name, topicname);

	return(0);
}

int destroy_tinfo(tinfo_t* topic) {
	if(!topic) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	destroy_fdarr(topic->fdarr);
	destroy_fdarr(topic->prod_fdarr);
	pthread_rwlock_destroy(&topic->m);

	if(topic->topic_name != NULL)
		free(topic->topic_name);

	free(topic->tid);
	free(topic);
	return(0);
}


ssize_t recvn(int fd, void* ptr, size_t n, int flag) {
	size_t nleft;
	ssize_t nread;
	
	nleft = n;
	while(nleft > 0) {
		if((nread = recv(fd, ptr, nleft, flag)) < 0) {
			if(nleft == n) return(-1);
			else break;
		} else if(nread == 0) {
			break;
		} else if(nread < 0 && errno == EINTR) {
			perror("recvn() interrupted");
			break;
		}

		nleft -= nread;
		ptr += nread;
	}

			
	return(n-nleft);
}


ssize_t sendn(int fd, void* ptr, size_t n, int flag) {
	size_t nleft;
	ssize_t nwritten;
	
	nleft = n;
	
	while(nleft > 0) {
		if((nwritten = send(fd, ptr, nleft, flag)) < 0) {
			if(nleft == n) return(-1);
			else break;
		} else if(nwritten == 0) {
			break; 
		} else if(nwritten < 0 && errno == EINTR) {
			perror("sendn() interrupted");
			break;
		}

		nleft -= nwritten;
		ptr += nwritten;
	}
	
	return(n - nleft);
}
