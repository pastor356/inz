#pragma once
#include <sys/epoll.h>

class IWaiter {
public:
	virtual int create() = 0;
	virtual int wait(int waitFd, 
			struct epoll_event* events, int maxevents, int timeout) = 0;
	virtual int ctl(int waitFd, int op, int fd,
				struct epoll_event* event) = 0;
};
