#include "header.h"
#include "msg.h"
#include "list_tinfo.h"
#include "client_info.h"
#include "fdarr.h"
#include "serializer.h"


int main(int argc, char** argv) {
	if(argc < 5) {
		print_help();
		exit(EXIT_FAILURE);
	}
	
	int  role = strtol(argv[3], NULL, 10);
	struct sockaddr_un sun;
	ssize_t slen, sersize;
	int fd;
	cinfo_t* client = NULL;
	char introduce_buf[VBUFSIZ];
	msg_t msg;
	sigset_t set;
	sigemptyset(&set);

	memset(introduce_buf, 0, VBUFSIZ);

	if(set_sigact(SIGINT, sigint, &set) < 0) {
		printf("%s:%s::set_sigact() failed\n", __FILE__, __func__);
	}

	if(init_msg(&msg, 0) < 0) {
		printf("%s:%s:: init_msg() failed\n", __FILE__, __func__);
		exit(EXIT_FAILURE);
	}

	if((fd = init_client(argv[1], SERVSOCK, &sun, &slen)) < 0) {
		printf("%s:%s:: init_client() failed\n", __FILE__, __func__);
		exit(EXIT_FAILURE);
	}

	client = create_cinfo();
	if(init_cinfo(client, argv[2], role) < 0) {
		printf("%s:%s:: init_cinfo() failed\n", __FILE__, __func__);
		exit(EXIT_FAILURE);
	}

	sersize = serialize_cinfo(client, VBUFSIZ, introduce_buf);

	if(connect(fd, (struct sockaddr*)&sun, slen) < 0) {
		perror("mqtt client: connect() failed");
		destroy_cinfo(client);
		init_msg(&msg, 1);
		exit(EXIT_FAILURE);
	}

	if(send(fd, introduce_buf, sersize, 0) < 0) {
		perror("send() failed");
	}

	// creating a header 
	char hdrbuf[VBUFSIZ];
	msg.hsize = snprintf(hdrbuf, VBUFSIZ, "%s%s%s", 
							argv[2], HDRDELIM, argv[4]);
	if(msg.hsize <= 0) {
		printf("%s:%s::snprintf() failed\n", __FILE__, __func__);
		exit(EXIT_FAILURE);
	}

	strcpy(msg.header, hdrbuf);

	char buf[VBUFSIZ];
	int buflen = 0;
	char serbuf[VBUFSIZ];
	ssize_t nbytessnd, nbytesrcv;
	
	memset(buf, 0, VBUFSIZ);
	memset(serbuf, 0, VBUFSIZ);
	while(quitflag != 1) {

		if(role == PROD) {
			
			if((buflen = get_msg(buf, VBUFSIZ)) < 0) {
				printf("%s:%s::get_msg() failed\n", __FILE__, __func__);
				if(EINTR == errno) break;
			}

			strcpy(msg.body, buf);
			msg.bsize = buflen;

			if((nbytessnd = send_msg(fd, &msg, buf, VBUFSIZ, 0)) < 0) {
				perror("send_msg() failed");
			}

		} else if(role == CONS) {

			if((nbytesrcv = recv_msg(fd, &msg, serbuf, VBUFSIZ, 0)) < 0) {
				perror("recv_msg() failed");
			} else if(nbytesrcv == 0) {
				printf("recv() == 0: connection closed\n");
				break;
			}

			print_msgt(&msg);

			memset(serbuf, 0, VBUFSIZ);
		}


	} // while(quitflag != 1)
	

	msg.hsize = 0;
	msg.bsize = 0;

	// send notification about disconnection to server
	send_msg(fd, &msg, buf, VBUFSIZ, 0);

	close(fd);

	if(init_msg(&msg, 1) < 0) {
		printf("%s:%s::init_msg(&msg, 1) failed\n", __FILE__, __func__);
	}

	destroy_cinfo(client);

	exit(EXIT_SUCCESS);
}
