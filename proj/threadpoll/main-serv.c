#include "serv.h"

Thread* tptr;
int listenfd, nthreads;
socklen_t addrlen;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char** argv) {
	if(argc != 4) {
		printf("usage: ./serv hostname service #threads\n");
		exit(EXIT_FAILURE);
	}

	size_t nthreads = atoi(argv[3]);

	listenfd = tcp_listen(argv[1], argv[2], &addrlen);
	if(listenfd < 0) {
		perror("tcp_listen() failed"); 
		exit(EXIT_FAILURE);
	}

	tptr = calloc(nthreads, sizeof(Thread));
	
	for(size_t i = 0; i < nthreads; ++i) {
		thread_make(i);
	}

	struct sigaction act;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	act.sa_handler = sigint;
	sigaction(SIGINT, &act, NULL); 

	for(;;) {
		pause();
	}

	return(0);
}
