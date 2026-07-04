#include "header.h"
#include "msg.h"
#include "serializer.h"

int init_serv(const char* path) {
	int fd, slen, err;
	struct sockaddr_un un;

	if(!path) {
		printf("init_serv(): bad param\n");
		err = -1;
		return(err);
	}

	if((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror("socket()");
		err = -2;
	}

	memset(&un, 0, sizeof(un));
	unlink(path);
	un.sun_family = AF_UNIX;
	strcpy(un.sun_path, path);
	slen = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path) + 1;

	if(bind(fd, (struct sockaddr*)&un, slen) < 0) {
		perror("bind() in init_serv()");
		err = -3;
	}

	return(fd);
}
 


void* thr_func(void* arg) {
	if(!arg) {
		printf("%s: bad params\n", __func__);
		pthread_exit(NULL);
	}

	int poll_timeout = -1;
	int poll_retval = 0;
	//char buf[VBUFSIZ];
	ssize_t nbytesrcv, nbytessnd;
	fdarr_t* copy_fdarr = NULL;

	int idx_to_pop = -1;

	tinfo_t* self_topic = (tinfo_t*) arg; 
	init_fdarr(&copy_fdarr, 10);

	char notif_buf[5];

	// NEW SERIALIZER:
	void* buf = init_serbuf(VBUFSIZ);
	memset(buf, 0, VBUFSIZ);
	msg_t msg;
	init_msg(&msg, 0);

	while(quitflag != 1) {

		
		pthread_rwlock_rdlock(&self_topic->m);

		if(copy_arr(self_topic->prod_fdarr, copy_fdarr) < 0) {
			printf("%s: copy_arr() failed\n", __func__);
		}
		
		pthread_rwlock_unlock(&self_topic->m);

		if((poll_retval = poll_fdarr(copy_fdarr, poll_timeout)) < 0) {
			perror("poll_fdarr() failed");

			if(self_topic->fdarr->sz == 0 && self_topic->prod_fdarr->sz == 1) {
				printf("topic %s is empty.\n", self_topic->topic_name);
				break;
			}

		} else if(poll_retval == 0) { // event on 
									  // thread-notification descroptor
			if(read(copy_fdarr->fdarr[poll_retval].fd, notif_buf, 5) < 0) {
				perror("read() in thread-notification fd");
			}

			printf("got a notification\n");
			continue;

		} else if(poll_retval > 0) {

			// RECV MSG
			if(copy_fdarr->fdarr[poll_retval].revents & (POLLIN | POLLHUP)) {
				
				nbytesrcv = recv_msg(copy_fdarr->fdarr[poll_retval].fd, &msg, buf, VBUFSIZ, 0);

				if(nbytesrcv < 0) {
					perror("recvn() failed");
				} else if(nbytesrcv == 0) {
					
					// remove prod client from array
					pthread_rwlock_wrlock(&self_topic->m);

					if((idx_to_pop = find_fd(self_topic->prod_fdarr,
									copy_fdarr->fdarr[poll_retval].fd)) < 0) {

						printf("%s: find_fd() failed (or fd is not found)\n", __func__);

					} else if(idx_to_pop >= 0) {

						if(delete_client(self_topic->prod_fdarr, idx_to_pop) < 0) {
							printf("%s: delete_client() failed\n", __func__);
						}
					}

					pthread_rwlock_unlock(&self_topic->m);

				} else if(nbytesrcv > 0) {

					// delete this
					printf("bytes received: %ld\n", nbytesrcv);

					// SEND TO ALL CONS CLIENTS
					pthread_rwlock_wrlock(&self_topic->m);

					if(copy_arr(self_topic->fdarr, copy_fdarr) < 0) {
						printf("%s: copy_arr() failed\n", __func__);
					}

					pthread_rwlock_unlock(&self_topic->m);
					
					for(size_t c = 0; c < copy_fdarr->sz; ++c) {

						nbytessnd = send_msg(copy_fdarr->fdarr[c].fd, &msg, buf, VBUFSIZ, 0);

						printf("bytes sent: %ld\n\n", nbytessnd);

						if(nbytessnd < 0) {
							printf("warning in %s: sendn() failed\n", __func__);
							if(errno == EPIPE) {
								
								// remove cons client from array
								printf("apparently, cons client disconnected\n");

								pthread_rwlock_wrlock(&self_topic->m);

								if((idx_to_pop = find_fd(self_topic->fdarr,
												copy_fdarr->fdarr[c].fd)) < 0) {
									printf("%s: find_fd() failed (or fd is not found)\n", __func__);

								} else if(idx_to_pop >= 0) {

									if(delete_client(self_topic->fdarr, idx_to_pop) < 0) {
										printf("%s: delete_client() failed\n", __func__);
									}	

								}
								
								pthread_rwlock_unlock(&self_topic->m);

								continue;
							}
						}
					}

					memset(buf, 0, VBUFSIZ);
				} // else if(nbytesrcv > 0)

			} // if(copy_fdarr->fdarr[poll_retval] & (POLLIN | POLLHUP))

		} // else if(poll_retval > 0)

	} // while(quitflag != 1)


	printf("thread is closing\n");

	destroy_fdarr(copy_fdarr);
	destroy_serbuf(buf);
	init_msg(&msg, 1);

	pthread_exit((void*)NULL);
}



int delete_client(fdarr_t* arr, ssize_t idx) {
	if(!arr || idx < 0) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	if(swap_back(arr, idx) < 0) {
		printf("%s: swap_back() failed\n", __func__);
		return(-1);
	}

	if(pop_backp(arr, NULL) < 0) {
		printf("%s: pop_back() failed\n", __func__);
		return(-1);
	}

	return(0);
}
