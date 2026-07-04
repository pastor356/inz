#include "header.h"
#include "fdarr.h"
#include "list_tinfo.h"
#include "msg.h"
#include "client_info.h"


int main(void) { 
	int fd, cfd;
	sigset_t set;
	char introduce_buf[VBUFSIZ];
	ssize_t nbytesrcv;
	pthread_attr_t attr; 

	msg_t newclient_msg;
	cinfo_t* newclient_info = NULL;

	tlist_t* root = NULL;
	if((root = create_node(NULL, NULL, NULL)) == NULL) {
		printf("create_node() failed\n");
	}

	if(init_msg(&newclient_msg, 0) < 0) {
		printf("init_msg() failed\n");
	}

	if((fd = init_serv(SERVSOCK)) < 0) {
		printf("%s: init_serv() failed\n", __func__);
	}

	if(sigemptyset(&set) < 0) {
		printf("%s: sigemptyset() failed\n", __func__);
	}

	if(set_sigact(SIGINT, sigint, &set) < 0) {
		printf("%s: set_sigact() failed\n", __func__);
	}

	if(set_sigact(SIGPIPE, SIG_IGN, &set) < 0) {
		printf("%s: set_sigact() failed\n", __func__);
	}

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	if(listen(fd, QLEN) < 0) {
		perror("listen()");
	}

	newclient_info = create_cinfo();
	int topic_idx = -1;
	tlist_t* currtopic = root;
	tinfo_t* newtopic = NULL;
	while(quitflag != 1) {
		if((cfd = accept(fd, NULL, NULL)) < 0) {
			perror("\naccept()");
			if(errno == EINTR) break;
		}

		// fill introduce_buf
		if((nbytesrcv = recv(cfd, introduce_buf, VBUFSIZ, 0)) < 0) {
			perror("recv() < 0 in main");
		} else if(nbytesrcv == 0) {
			printf("len(introduce_buf) == 0: new client sent empty msg\n");
		}

		// fill client_info
		if(deserialize_cinfo(newclient_info, introduce_buf) < 0) {
			printf("%s: deserialize_cinfo() failed\n", __func__);
		}

		
		// check if topic exists
		if((topic_idx = checkif_topic_exists(root, newclient_info->topic_name)) > 0) {

			for(size_t i = 0; i < topic_idx; ++i) {
				currtopic = currtopic->next;
			}

			if(newclient_info->role == PROD) {

				pthread_rwlock_wrlock(&currtopic->topic->m);
				push_backf(currtopic->topic->prod_fdarr, cfd);
				pthread_rwlock_unlock(&currtopic->topic->m);

			} else if(newclient_info->role == CONS) {

				pthread_rwlock_wrlock(&currtopic->topic->m);
				push_backf(currtopic->topic->fdarr, cfd);
				pthread_rwlock_unlock(&currtopic->topic->m);

			} else {
				printf("unknown role for client. skipping\n");
				continue;
			}

			// notify thread about new client
			if(write(currtopic->topic->notif[1], "U", 2) < 0) {
				perror("write() while notifying thread about new client");
			}

		} else if(topic_idx < 0) { // topic doesn't exists => time to create one
			newtopic = create_tinfo();
			init_tinfo(newtopic, newclient_info->topic_name);

			if(pipe(newtopic->notif) < 0) perror("pipe()");
			push_backf(newtopic->prod_fdarr, newtopic->notif[0]);

			if(newclient_info->role == PROD) {
				push_backf(newtopic->prod_fdarr, cfd);
			} else if(newclient_info->role == CONS){
				push_backf(newtopic->fdarr, cfd);
			} else {
				printf("unknown role for client. skipping\n");
				continue;
			}

			push_back(&root, newtopic);
			
			// create thread for topic:
			pthread_create(newtopic->tid, &attr, thr_func, newtopic);

			
		}


		// endcycle routine
		newclient_info->role = 0;
		memset(newclient_info->topic_name, 0, VBUFSIZ);
		currtopic = root;

	} // while(quitflag != 1)




	printf("quitflag == 1\ncleaning stuff\n");

	const char* notify_thread = "C";

	if(root->next != NULL) {
		currtopic = root->next;

		if(write(currtopic->topic->notif[1], notify_thread, 1) < 0) {
			perror("write() while sending cancel byte");
		}

		pthread_join(*currtopic->topic->tid, NULL);

		destroy_tinfo(currtopic->topic);

		while(currtopic->next != NULL) {
			currtopic = currtopic->next;

			if(write(currtopic->topic->notif[1], notify_thread, 1) < 0) {
				perror("write() while sending cancel byte");
			}

			pthread_join(*currtopic->topic->tid, NULL);
			destroy_tinfo(currtopic->topic);
		}
	}


	init_msg(&newclient_msg, 1);

	destroy_cinfo(newclient_info);
	free_list(&root);

	exit(EXIT_SUCCESS);
}

