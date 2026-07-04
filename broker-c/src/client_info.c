#include "client_info.h"


cinfo_t* create_cinfo(void) {
	cinfo_t* newclient = (cinfo_t*) malloc(sizeof(cinfo_t));

	if(!newclient) {
		printf("%s: malloc() failed\n", __func__);
		return(NULL);
	}

	newclient->topic_name = (char*) malloc(VBUFSIZ);
	if(!newclient->topic_name) {
		printf("%s: malloc() failed\n", __func__);
		if(newclient) free(newclient);
		return(NULL);
	}

	return(newclient);
}


int init_cinfo(cinfo_t* client, char* topic_name, int role) {
	if(!client || !topic_name || (role != PROD && role != CONS) ) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	strcpy(client->topic_name, topic_name);
	client->role = role;

	return(0);
}

int destroy_cinfo(cinfo_t* client) {
	if(!client) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	if(client->topic_name)	free(client->topic_name);
	client->topic_name = NULL;

	if(client) free(client);
	client = NULL;
	

	return(0);
}


int serialize_cinfo(cinfo_t* src, size_t srcsize, char* dst) {
	if(!src || !dst) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	memset(dst, 0, VBUFSIZ);
	ssize_t sersize = snprintf(dst, srcsize,
		"%d%s%s",
		src->role,
		DELIM_CINFO,
		src->topic_name
	);

	if(sersize >= VBUFSIZ) {
		printf("serbuf overflow in %s(). no msg serialized.\n", __func__);
		return(-2);
	} else if(sersize < 0) {
		printf("formating error in serialize()\n");
		return(-3);
	}

	return(sersize);
}


int deserialize_cinfo(cinfo_t* dst, char* src) {
	if(!src || !dst) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	char* saveptr = NULL;
	char* tok = strtok_r(src, DELIM_CINFO, &saveptr);
	if(tok) dst->role = strtol(tok, NULL, 10);

	tok = strtok_r(NULL, DELIM_CINFO, &saveptr);
	if(tok) strcpy(dst->topic_name, tok);

	return(0);
}
