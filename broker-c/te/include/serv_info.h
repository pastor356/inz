#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <string.h>

// initial topicset size
#define TS_SIZE 5

typedef struct _topic {
	char*		 _topicname;
	uint32_t	 _prodlen;
	uint32_t	 _conslen;
} _topic;

_topic* create_topic(void);
int init_topic(_topic* topic, const char* topicname, size_t prodlen, size_t conslen);
int destroy_topic(_topic* topic);




typedef struct serv_info {
	_topic* topicset;
	size_t topic_count;
	char* servname;
} serv_info_t;

serv_info_t* create_servinfo(void);
int init_servinfo(serv_info_t* si, const char* servname);
int push_back_si(serv_info_t* si, const char* topicname, size_t plen, size_t clen);
int destroy_servinfo(serv_info_t* si);
