#include "EPollWaiter.hpp"

int EPollWaiter::wait(int waitFd, 
			struct epoll_event* events, int maxevents, int timeout) {
	return(epoll_wait(waitFd, events, maxevents, timeout));
}


int EPollWaiter::create() {
	return(epoll_create(1));
}


int EPollWaiter::ctl(int waitFd, int op, int fd, struct epoll_event* event) {
	return(epoll_ctl(waitFd, op, fd, event));
}
