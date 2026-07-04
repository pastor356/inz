#include "serv_info.h"

void sigsegv(int signo) {
	if(signo == SIGSEGV) printf("\ncaught SIGSEGV\n"); 
}
	


void* alloc(size_t nbytes) {
	if(nbytes == 0) return NULL;

	int err;
	jmp_buf buf; void* t = NULL;

	struct sigaction act, oldact;
	act.sa_handler = sigsegv;
	sigfillset(&act.sa_mask);
	sigdelset(&act.sa_mask, SIGSEGV);
	act.sa_flags = 0;

	sigaction(SIGSEGV, &act, &oldact);

	if((err = sigsetjmp(buf, 1)) == 1) {
		printf("%s failed to allocate memory\n", __func__);
	} else if(err == 2) {
		printf("%s failed to allocate memory second time\n", __func__);
	} else if(err == 3) {
		printf("%s failed to allocate memory third time\n", __func__);
		printf("something is wrong"
				"emergency exit");
		sigaction(SIGSEGV, &oldact, NULL);
		t = NULL;
		return(t);
	}

	t = malloc(nbytes);

	if(t == NULL && err ==  0) {
		siglongjmp(buf, 1);
	} else if(t == NULL && err == 1) {
		siglongjmp(buf, 2);
	} else if(t == NULL && err == 2) {
		siglongjmp(buf, 3);
	}

	sigaction(SIGSEGV, &oldact, NULL);

	memset(t, 0, nbytes);

	return(t);
}



_topic* create_topic(void) {
	_topic* t = (_topic*) alloc(sizeof(_topic));

	if(t == NULL) printf("alloc() in %s failed\n", __func__);

	return(t);
}





int init_topic(_topic* topic, const char* topicname, size_t prodlen, size_t conslen) {
	if(!topicname) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	if(!topic) topic = create_topic();

	size_t len = strlen(topicname);
	topic->_topicname = strndup(topicname, len + 1); // heap allocation

	topic->_prodlen = prodlen;
	topic->_conslen = conslen;

	return(0);
}



int destroy_topic(_topic* topic) {
	if(!topic || !(topic->_topicname)) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	free(topic->_topicname);

	return(0);
}




serv_info_t* create_servinfo(void) {
	serv_info_t* si = (serv_info_t*) alloc(sizeof(serv_info_t));

	if(!si) {
		printf("%s: alloc() failed to allocate memory\n", __func__);
	}

	memset(si, 0, sizeof(serv_info_t));

	return(si);
}



int init_servinfo(serv_info_t* si, const char* servname) {
	if(!servname) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	si->topicset = (_topic*) alloc(TS_SIZE * sizeof(_topic));
	si->topic_count = 0;

	if(!si->topicset) printf("%s: alloc() failed to allocate memory\n", __func__);

	si->servname = strndup(servname, strlen(servname) + 1); // heap allocation

	return(0);
}



int push_back_si(serv_info_t* si, const char* topicname, size_t plen, size_t clen) { 
	if(!si || !topicname) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	if(TS_SIZE == si->topic_count) {
		_topic* newt = (_topic*) alloc(si->topic_count * 2 * sizeof(_topic));

		// copy data to new set
		for(size_t i = 0; i < si->topic_count; ++i) {
			memcpy(&newt[i], &si->topicset[i], sizeof(si->topicset[i]));
		}

		// delete oldset
		free(si->topicset);
		
		// rewrite set
		si->topicset = newt;
	} 

	if(init_topic(&si->topicset[si->topic_count], topicname, plen, clen) < 0) {
		printf("%s: init_topic() failed\n", __func__);
	}

	si->topic_count++;

	return(0);
}



int destroy_servinfo(serv_info_t* si) {
	if(!si) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	if(si->servname) free(si->servname);

	for(size_t i = 0; i < si->topic_count; ++i) {
		destroy_topic(&si->topicset[i]);
	}

	if(si->topicset) free(si->topicset);

	free(si);

	return(0);
}
