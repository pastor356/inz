#include "header.h"
#include "serializer.h"
#include "client_info.h"

int init_client(const char* path, const char* servpath, struct sockaddr_un* sun, ssize_t* slen) {
	int fd, len;
	struct sockaddr_un un;

	if(!path || !sun || !slen) {
		printf("init_client(): bad params\n");
		return(-1);
	} 

	if((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror("socket() in init_client()");
		return(-2);
	}

	memset(&un, 0, sizeof(un));
	unlink(path);
	un.sun_family = AF_UNIX;
	strcpy(un.sun_path, path);
	len = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path) + 1;

	if(bind(fd, (struct sockaddr*)&un, len) < 0) {
		perror("bind() in init_client()");
		return(-2);
	}

	memset(sun, 0, sizeof(struct sockaddr_un));
	sun->sun_family = AF_UNIX;
	//unlink(servpath);
	strcpy(sun->sun_path, servpath);
	*slen = offsetof(struct sockaddr_un, sun_path) + strlen(servpath) + 1;

	return(fd);
}



int get_msg(char* buf, size_t sz) {
	if(!buf) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	memset(buf, 0, sz);
	int len;

	if((len = read(STDIN_FILENO, buf, sz-1)) < 0) {
		perror("read(STDIN_FILENO) failed");
	}

	if(buf[len - 1] == '\n') {
		buf[len - 1] = '\0';
	}

	return(len);
}


void print_help(void) {
	printf("usage: client <client.sock> <topic_name> <role> <client_name>\n"
			"where\n\t(char*) client.sock -- path to client's socket,\n"
			"\t(char*) topic_name is name of the topic you want to subscribe to\n"
			"\t(int) role is role of client. 1 for PROD and 0 for CONS\n"
			"\t(char*) client_name is name of client\n");
	return;
}


//int gettopicinfo(const char* serv, const char* topic, ) {

