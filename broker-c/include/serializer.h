#include "header.h"
#include "msg.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXBUFBYTES 128

void* init_serbuf(size_t nbytes);

int serialize_metadata(void* buf, size_t bufsiz, uint32_t hsiz, uint32_t bsiz);
int deserialize_metadata(void* buf, size_t bufsiz, uint32_t* hsiz, uint32_t* bsiz);

int serialize_data(void* buf, size_t bufsiz, 
					const char* hdr, uint32_t hsiz, 
					const char* body, uint32_t bsiz);

int deserialize_data(void* buf, size_t bufsiz, 
					char* hdr, uint32_t hsiz,
					char* body, uint32_t bsiz);


int destroy_serbuf(void* buf);

int send_msg(int fd, const msg_t* msg, void* buf, size_t bufsiz, int flags);
int recv_msg(int fd, msg_t* msg, void* buf, size_t bufsiz, int flags);

