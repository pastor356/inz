#include "fdarr.h"


int reserve(fdarr_t* arr, size_t cap) {
	if(!arr) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	struct pollfd* newarr = (struct pollfd*) malloc(cap * sizeof(struct pollfd));
	if(!newarr) {
		perror("init_fdarr(): malloc() failed\n");
	}

	if(arr->fdarr != NULL) {
		for(size_t i = 0; i < arr->sz; ++i) {
			newarr[i] = arr->fdarr[i];
		}
	
		free(arr->fdarr);
	}

	arr->fdarr = newarr;
	arr->cap = cap;

	return(0);
}



int init_fdarr(fdarr_t** arr, size_t c) {
	if(!arr) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	*arr = (fdarr_t*) malloc(sizeof(fdarr_t));
	
	if(*arr == NULL) {
		printf("%s: malloc() failed\n", __func__);
		return(-1);
	}

	(*arr)->fdarr = NULL;
	if(reserve(*arr, c) < 0) return(-1);
	(*arr)->sz = 0;

	return(0);
}


int destroy_fdarr(fdarr_t* arr) {
	if(!arr && !arr->fdarr) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	if(arr->fdarr) free(arr->fdarr);
	arr->fdarr = NULL;
	arr->sz = arr->cap = 0;

	free(arr);
	
	return(0);
}


int push_backp(fdarr_t* arr, struct pollfd* fd) {
	if(!arr && !arr->fdarr && !fd) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	if(arr->sz == arr->cap) {
		if(reserve(arr, arr->cap * 2) < 0) return(-1);
	}

	arr->fdarr[arr->sz++] = *fd;

	return(0);
}


int push_backf(fdarr_t* arr, int fd) {
	if(!arr && !arr->fdarr) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	if(arr->sz == arr->cap) {
		if(reserve(arr, arr->cap * 2) < 0) return(-1);
	}
	
	arr->fdarr[arr->sz].fd = fd;
	arr->fdarr[arr->sz++].events = POLLIN;

	return(0);
}


int swap_back(fdarr_t* arr, int idx) {
	if(!arr || idx < 0) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	if(idx > arr->sz) {
		printf("%s: idx out of range\n", __func__);
		return(-1);
	}

	struct pollfd tmp = {
		.fd = arr->fdarr[arr->sz - 1].fd,
		.events = arr->fdarr[arr->sz - 1].events
	};

	arr->fdarr[arr->sz - 1] = (struct pollfd) {
		.fd = arr->fdarr[idx].fd,
		.events = arr->fdarr[idx].events
	};
	
	arr->fdarr[idx] = (struct pollfd) {
		.fd = tmp.fd,
		.events = tmp.events
	};



	return(0);
}


int pop_backp(fdarr_t* arr, struct pollfd* poped_obj) {
	if(!arr) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	if(poped_obj) {
		*poped_obj = (struct pollfd) {
			.fd = arr->fdarr[arr->sz - 1].fd,
			.events = arr->fdarr[arr->sz - 1].events
		};
	}

	arr->sz--;

	return(0);
}


int copy_arr(fdarr_t* src, fdarr_t* dst) {
	if(!src || !dst) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}
	
	if(src->sz > dst->cap) {
		if(reserve(dst, src->sz * 2) < 0) {
			printf("%s: reserve() failed\n", __func__);
			return(-1);
		}
	}

	for(size_t i = 0; i < src->sz; ++i) {
		dst->fdarr[i] = src->fdarr[i];
	}

	dst->sz = src->sz;


	return(0);
}


int find_fd(fdarr_t* arr, unsigned int fd) {
	if(!arr) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}
	size_t i = 0;
	for(; i < arr->sz; ++i) {
		if(fd == arr->fdarr[i].fd) return(i);
	}

	return(-1);
}


int poll_fdarr(fdarr_t* arr, int poll_timeout) {
	if(!arr || !arr->fdarr) {
		printf("%s: bad params\n", __func__);
		return(-1);
	}

	int poll_retval = -1;
	size_t i = 0;

	if((poll_retval = poll(arr->fdarr, arr->sz, poll_timeout)) < 0) {
		perror("poll() in poll_fdarr()");
		return(-1);
	} else if(poll_retval > 0) {

		// find descriptor with revents := (POLLIN || POLLHUP)
		for(; i < arr->sz; ++i) {
			if(arr->fdarr[i].revents & (POLLIN | POLLHUP)) return(i);
		}
	} 

	return(-1);
}
