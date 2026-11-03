#pragma once
#include "IWaiter.hpp"


class EPollWaiter: public IWaiter {
public:
	int create() override;
	int wait(int waitFd, struct epoll_event* events, 
			int maxevents, int timeout) override;
	int ctl(int waitFd, int op, int fd,
				struct epoll_event* event);
};
