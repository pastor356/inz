#include "msg.h"



int init_msg(msg_t* msg, int del) {
	if(!msg) {
		printf("init_msg(): bad params\n");
		return(-1);
	}

	if(del == 1) {
		if(msg->header != NULL) free(msg->header);
		if(msg->body != NULL) free(msg->body);
		msg->header = NULL;
		msg->body = NULL;
		msg->bsize = 0;
		msg->hsize = 0;
		msg->flags = 0;
		return(1);
	}

	msg->header = (char*) malloc(VBUFSIZ);
	msg->body = (char*) malloc(VBUFSIZ);

	if(msg->header == NULL || msg->body == NULL) {
		perror("malloc() in init_msg()");
		return(-2);
	}

	msg->hsize = 0;
	msg->bsize = 0;
	msg->flags = 0;

	return(0);
}


int deserialize(char* src, msg_t* dst) {
	if(!dst || !src) {
		printf("deserialize(): bad params\n");
		return(-1);
	}

	char* saveptr = NULL;
	int i = 0;

	char* tok = strtok_r(src, DELIM, &saveptr);
	if(tok) {
		dst->hsize = strtol(tok, NULL, 10);
		++i;
	}

	tok = strtok_r(NULL, DELIM, &saveptr);
	if(tok) {
		dst->bsize = strtol(tok, NULL, 10);
		++i;
	}

	tok = strtok_r(NULL, DELIM, &saveptr);
	if(tok) {
		strcpy(dst->header, tok);
		++i;
	}

	tok = strtok_r(NULL, DELIM, &saveptr);
	if(tok) {
		strcpy(dst->body, tok);
		++i;
	}

	tok = strtok_r(NULL, DELIM, &saveptr);
	if(tok) {
		dst->flags = strtol(tok, NULL, 10);
		++i;
	}

	return(i);
}



int serialize(msg_t* src, char* dst, size_t dstsize) {
	if(!src || !dst) {
		printf("serialize(): bad params\n");
		return(-1);
	}

	ssize_t sersize = snprintf(dst, dstsize,
				"%d%s%d%s%s%s%s%s%d",
				src -> hsize,
				DELIM,
				src->bsize,
				DELIM,
				src->header,
				DELIM,
				src->body,
				DELIM,
				src->flags);

	if(sersize >= VBUFSIZ) {
		printf("serbuf overflow in serialize()."
						"no msg serialzed\n");
		return(-2);
	} else if(sersize < 0) {
		printf("formating error in serialize()\n");
		return(-3);
	}

	return(sersize);
}



int deserialize_hdr(char* src, msg_t* dst) {
	if(!src || !dst) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	char* saveptr = NULL;
	char* tok = NULL;

	tok = strtok_r(src, DELIM, &saveptr);
	if(tok) dst->hsize = strtol(tok, NULL, 10);
	else dst->hsize = 0;

	tok = strtok_r(NULL, DELIM, &saveptr);
	if(tok) strcpy(dst->header, tok);
	else memset(dst->header, 0, VBUFSIZ);

	return(0);
}

int deserialize_body(char* src, msg_t* dst) {
	if(!src || !dst) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	char* saveptr = NULL;
	char* tok = NULL;

	tok = strtok_r(src, DELIM, &saveptr);
	if(tok) dst->bsize = strtol(tok, NULL, 10);
	else dst->hsize = 0;

	tok = strtok_r(NULL, DELIM, &saveptr);
	if(tok) strcpy(dst->body, tok);
	else memset(dst->body, 0, VBUFSIZ);

	return(0);
}


int serialize_hdr(msg_t* src, char* dst, size_t dstsize) {
	if(!src || !dst) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	ssize_t bufsize = snprintf(dst, dstsize, 
			"%d%s%s%s",
			src->hsize,
			DELIM,
			src->header,
			DELIM);

	if(bufsize >= VBUFSIZ) {
		printf("serbuf overflow in serialize()."
						"no msg serialzed\n");
		return(-2);
	} else if(bufsize < 0) {
		printf("formating error in serialize()\n");
		return(-3);
	}

	return(bufsize);
}



int serialize_body(msg_t* src, char* dst, size_t dstsize) {
	if(!src || !dst) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	ssize_t bufsize = snprintf(dst, dstsize, 
			"%d%s%s",
			src->bsize,
			DELIM,
			src->body);

	if(bufsize >= VBUFSIZ) {
		printf("serbuf overflow in serialize()."
						"no msg serialzed\n");
		return(-2);
	} else if(bufsize < 0) {
		printf("formating error in serialize()\n");
		return(-3);
	}

	return(bufsize);
}



int print_msgt(msg_t* msg) {
	if(!msg) {
		printf("print_msgt(): bad params\n");
		return(-1);
	}

	char topic[VBUFSIZ];
	char name[VBUFSIZ];
	memset(topic, 0, VBUFSIZ);
	memset(name, 0, VBUFSIZ);

	char* tok = NULL;
	char* saveptr = NULL;

	tok = strtok_r(msg->header, HDRDELIM, &saveptr);
	if(tok) strncpy(topic, tok, VBUFSIZ);

	tok = strtok_r(NULL, HDRDELIM, &saveptr);
	if(tok) strncpy(name, tok, VBUFSIZ);

	printf("client [%s] have a msg on topic [%s]\n",
					name, topic);

	//printf("[%s] is header with len [%u]\n",
	//				msg->header, msg->hsize);
	printf("[%s] is body with len [%u]\n\n",
					msg->body, msg->bsize);

	return(0);
}
