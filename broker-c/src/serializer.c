#include "serializer.h"


void* init_serbuf(size_t nbytes) {
	if(nbytes > MAXBUFBYTES) {
		printf("%s: bad params\n", __func__); return(NULL);
	}

	void* buf = malloc(nbytes + 1);
	if(buf == NULL) {
		printf("%s: malloc() failed\n", __func__);
		return(NULL);
	}

	return(buf);
}

int destroy_serbuf(void* buf) {
	if(buf == NULL) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	free(buf);

	return(0);
}

int serialize_metadata(void* buf, size_t bufsiz, uint32_t hsiz, uint32_t bsiz) {
	if(!buf || bufsiz < sizeof(hsiz)*2 + 1) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	memcpy(buf, &hsiz, sizeof(hsiz));
	memcpy(buf + sizeof(hsiz), &bsiz, sizeof(bsiz));


	return(strlen((char*)buf));
}


int deserialize_metadata(void* buf, size_t bufsiz, uint32_t* hsiz, uint32_t* bsiz) {
	if(!buf || !hsiz || !hsiz || bufsiz < 2*sizeof(uint32_t) + 1) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	memcpy(hsiz, buf, sizeof(uint32_t));
	memcpy(bsiz, (buf + sizeof(uint32_t)), sizeof(uint32_t));

	return(0);
}



int serialize_data(void* buf, size_t bufsiz, 
					const char* hdr, uint32_t hsiz, 
					const char* body, uint32_t bsiz) {
	if(!buf || bufsiz < hsiz + bsiz + 1) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	ssize_t n = 0;

	n = snprintf(NULL, n, "%s%s", hdr, body);

	if(n < 0) {
		perror("snprintf() failed in serialize_metadata()");
		return(-1);
	}

	++n;

	if((n = snprintf(buf, n, "%s%s", hdr, body)) < 0) {
		perror("snprintf() failed in serialize_metadata()");
		return(-1);
	}

	return(n);
}



int deserialize_data(void* buf, size_t bufsiz, 
					char* hdr, uint32_t hsiz,
					char* body, uint32_t bsiz) {
	if(!buf || !hsiz || !bsiz || bufsiz < hsiz + bsiz) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}


	memcpy(hdr, buf, hsiz);
	hdr[hsiz] = '\0';

	memcpy(body, buf + hsiz, bsiz);
	body[bsiz] = '\0';

	return(0);
}



int send_msg(int fd, const msg_t* msg, void* buf, size_t bufsiz, int flags) {
	if(fd < 0 || !msg || msg->hsize + msg->bsize >= bufsiz) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	ssize_t n, nbytessnd;
	
	serialize_metadata(buf, bufsiz, msg->hsize, msg->bsize);

	if((nbytessnd = sendn(fd, buf, 8, flags)) < 0) {
		perror("sendn() in send_msg() failed");
		return(-1);
	}

	memset(buf, 0, bufsiz);

	n = serialize_data(buf, bufsiz, msg->header, msg->hsize,
					msg->body, msg->bsize);

	if(n < 0) return(-1);

	if((nbytessnd += sendn(fd, buf, n, flags)) < 8) {
		perror("sendn() in send_msg() failed");
		return(-1);
	}

	memset(buf, 0, bufsiz);

	return(nbytessnd);
}



int recv_msg(int fd, msg_t* msg, void* buf, size_t bufsiz, int flags) {
	if(fd < 0 || !msg) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	ssize_t nbytesrcv = 0;

	if((nbytesrcv = recvn(fd, buf, 8, flags)) < 0) {
		perror("recvn() in recv_msg() failed");
		return(-1);
	}

	deserialize_metadata(buf, bufsiz, &msg->hsize, &msg->bsize);

	if(msg->hsize == 0 && msg->bsize == 0) {
		return(0);
	}

	memset(buf, 0, bufsiz);

	if((nbytesrcv += recvn(fd, buf, msg->hsize + msg->bsize - 1, flags)) < 0) {
		perror("recvn() in recv_msg() failed");
		return(-1);
	}

	deserialize_data(buf, bufsiz, msg->header, msg->hsize,
			msg->body, msg->bsize);

	memset(buf, 0, bufsiz);

	return(nbytesrcv);
}
