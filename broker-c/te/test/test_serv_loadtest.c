#include "header.h"
#include "./test_h/test.h"

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
int thread_no = 0;
pthread_attr_t attr;
size_t nmsg = 1;

void sigrtmin(int signo) {
	if(signo == SIGRTMIN) printf("\nsigrtmin\n");
}


void* prod_func(void* arg) {
	int thread = 0;
	struct sockaddr_un sun;
	ssize_t slen;
	msg_t msg;

	int err;
	pthread_mutex_lock(&m);
	thread = thread_no++;
	pthread_mutex_unlock(&m);

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

	char self_sock[VBUFSIZ];
	snprintf(self_sock, VBUFSIZ, "./loadtest/client%d.sock", thread);

	int self_fd = init_client(self_sock, SERVSOCK, &sun, &slen);
	init_msg(&msg, 0);

	int nonblock = 1;
	setsockopt(self_fd, SOL_SOCKET, SOCK_NONBLOCK, &nonblock, sizeof(int));


	char topic[VBUFSIZ];
	snprintf(topic, VBUFSIZ, "topic%d", thread);

	size_t cons_len = 2;
	pthread_t cons[cons_len];
	for(size_t i = 0; i < cons_len; ++i) {
		// every prod creates cons's for himself
		// thread passed to cons_func() is topic_no
		pthread_create(&cons[i], &attr, cons_func, &thread); 
		sleep(1);
	}


	cinfo_t *client = create_cinfo();
	init_cinfo(client, topic, 1);

	char introduce_buf[VBUFSIZ];
	size_t sersize = serialize_cinfo(client, VBUFSIZ, introduce_buf);


	err = connect(self_fd, (struct sockaddr*) &sun, slen);

	if(err < 0) goto exit;

	if(send(self_fd, introduce_buf, sersize, 0) < 0) goto exit;

	fsync(self_fd);

	printf("sent introduction_buf: %s\n", introduce_buf);

	
	ssize_t n = 0;
	char hdrbuf[VBUFSIZ];
	n = snprintf(NULL, n, "%s%s%s%d", topic, HDRDELIM, "cli", thread);
	++n;
	msg.hsize = snprintf(hdrbuf, n, "%s%s%s%d", topic, HDRDELIM, "cli", thread);
	strcpy(msg.header, hdrbuf);
	n = 0;

	char bodybuf[VBUFSIZ];
	memset(bodybuf, 0, VBUFSIZ);
	char serbuf[VBUFSIZ];

	// NEW SERIALIZER
	void* buf = init_serbuf(VBUFSIZ);
	

	size_t srsize;
	for(size_t i = 0; i < nmsg; ++i) {
		n = snprintf(NULL, n, "msg no %ld", i);
		++n;

		msg.bsize = snprintf(bodybuf, n, "msg no %ld", i);
		strcpy(msg.body, bodybuf);

		//srsize = serialize(&msg, serbuf, VBUFSIZ);
		//sendn(self_fd, serbuf, strlen(serbuf), 0);

		send_msg(self_fd, &msg, buf, VBUFSIZ, 0);

		fsync(self_fd);
		printf("sent msg: %s\n", serbuf);

		memset(bodybuf, 0, VBUFSIZ);
		memset(serbuf, 0, VBUFSIZ);
		n = 0;
		sleep(1);
	}

	goto exit;

exit:
	sleep(3);
	for(size_t i = 0; i < cons_len; ++i) {
		printf("prod_func(): joining cons's threads: %ld\n", i);
		pthread_kill(cons[i], SIGRTMIN);
		pthread_join(cons[i], NULL);
	}

	destroy_serbuf(buf);

	destroy_cinfo(client);
	init_msg(&msg, 1);
	pthread_exit(NULL);
}


void* cons_func(void* arg) {
	int topic_no = *(int*) arg;
	int thread = 0;
	struct sockaddr_un sun;
	ssize_t slen;
	msg_t msg;

	char topic[VBUFSIZ];
	snprintf(topic, VBUFSIZ, "topic%d", topic_no);

	sigset_t set;
	sigemptyset(&set);
	pthread_sigmask(SIG_SETMASK, &set, NULL);


	struct sigaction act;
	act.sa_handler = sigrtmin;
	act.sa_mask = set;
	act.sa_flags = 0;
	sigaction(SIGRTMIN, &act, NULL);

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

	pthread_mutex_lock(&m);
	thread = thread_no++;
	pthread_mutex_unlock(&m);

	char self_sock[VBUFSIZ];
	snprintf(self_sock, VBUFSIZ, "./loadtest/client%d.sock", thread);

	int self_fd = init_client(self_sock, SERVSOCK, &sun, &slen);
	init_msg(&msg, 0);

	cinfo_t *client = create_cinfo();
	init_cinfo(client, topic, 0);

	char introduce_buf[VBUFSIZ];
	size_t sersize = serialize_cinfo(client, VBUFSIZ, introduce_buf);

	printf("CONSCLI: introduce_buf: %s\n", introduce_buf);

	int err;
	err = connect(self_fd, (struct sockaddr*) &sun, slen);
	if(err < 0) goto exit;

	sendn(self_fd, introduce_buf, sersize, 0);
	printf("sent introduction_buf: %s\n", introduce_buf);

	char recvbuf[VBUFSIZ];
	ssize_t nbytesrcv = 0;


	// NEW SERIALIZER
	void* buf = init_serbuf(VBUFSIZ);

	for(size_t i = 0; i < nmsg; ++i) {
		//memset(recvbuf, 0, VBUFSIZ);
		//nbytesrcv = recv(self_fd, recvbuf, VBUFSIZ, 0);

		recv_msg(self_fd, &msg, buf, VBUFSIZ, 0);

		fsync(self_fd);

		print_msgt(&msg);
		//printf("recv msg: %s\n", recvbuf);

		if(nbytesrcv < 0) {
			if(errno == EINTR) goto exit;
		}

	}

	goto exit;

exit:
	destroy_serbuf(buf);
	init_msg(&msg, 1);
	destroy_cinfo(client);
	pthread_exit(NULL);
}


void test_serv_loadtest(void) {
	size_t prod_len = 1;

	pthread_t prod_arr[prod_len];

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_mutex_init(&m, NULL);

	for(size_t i = 0; i < prod_len; ++i) {
		pthread_create(&prod_arr[i], &attr, prod_func, NULL);
	}


	for(size_t i = 0; i < prod_len; ++i) {
		pthread_join(prod_arr[i], NULL);
	}

	pthread_mutex_destroy(&m);
	pthread_attr_destroy(&attr);

	exit(EXIT_SUCCESS);
}


