#include "TimerQueue.hpp"


TimerQueue::TimerQueue() {
	this->timerFd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);

	if(this->timerFd < 0) {
		logError << 
			std::string{"TimerQueue::TimerQueue() failed: timerfd_create() returned < 0"};
	}
}

int TimerQueue::getDescriptor() const {
	return(this->timerFd);
}

ITimerQueue::timerId TimerQueue::addTimer(ITimerQueue::timePoint tp, ITimerQueue::callback cb) {
	const char* methodName = "TimerQueue::addTimer(timePoint, callback)";

	++thisTimerId;
	int isEarliest = timePointToCb.empty() || timePointToCb.begin()->first > tp; 

	auto it = timePointToCb.emplace(tp, std::make_pair(thisTimerId, cb));

	auto ret = idToTime.try_emplace(thisTimerId, tp);

	if(!ret.second) {
		logError << std::string{methodName} + " failed: idToTime.try_emplace() failed";
		return(0u);
	}

	if(isEarliest) updateSystemTimer(tp);

	return(thisTimerId);
}

void TimerQueue::cancelTimer(ITimerQueue::timerId const& tid) {
    
    auto itId = idToTime.find(tid);
    if (itId == idToTime.end()) {
        return; 
    }

	ITimerQueue::timePoint tp = itId->second;
    idToTime.erase(itId); 

    auto range = timePointToCb.equal_range(tp);
    for (auto it = range.first; it != range.second; ++it) {
        if (it->second.first == tid) {
            bool wasEarliest = (it == timePointToCb.begin());

            timePointToCb.erase(it); 

            if (wasEarliest) {
                if (!timePointToCb.empty()) {
                    updateSystemTimer(timePointToCb.begin()->first);
                } else {
                    stopSystemTimer(); 
                }
            }
            break;
        }
    }
}


IConnection::Status TimerQueue::recv(Packet& packet, int flag) {
	uint64_t buf;
	
	(void) flag;
	(void) packet;

	ssize_t nread = ::read(timerFd, &buf, sizeof(buf));

	std::vector<TimerQueue::callback> cbs;
	auto now = TimerQueue::clock::now();

	auto it = timePointToCb.begin();
	while(!timePointToCb.empty() &&  timePointToCb.begin()->first <= now) {
		it = timePointToCb.begin();

		idToTime.erase(it->second.first);
		cbs.push_back(std::move(it->second.second));
		timePointToCb.erase(it);
	}

	if(!timePointToCb.empty()) {
		updateSystemTimer(timePointToCb.begin()->first);
	} else stopSystemTimer();


	for(auto& cb: cbs) {
		if(cb) cb();
	}

	return(IConnection::Status::SUCCESS);
}



IConnection::Status TimerQueue::send(Packet& packet, int flag) {
	(void) packet;
	(void) flag;
	return(IConnection::Status::SUCCESS);
}


void TimerQueue::updateSystemTimer(TimerQueue::timePoint tp) {
	struct itimerspec ts{};
	auto now = TimerQueue::clock::now();
	auto dur = tp - now;

	if(dur.count() <= 0) {
		ts.it_value.tv_nsec = 1000;
	} else {
		auto sec = std::chrono::duration_cast<std::chrono::seconds>(dur);
		auto nsec = std::chrono::duration_cast<std::chrono::nanoseconds>(dur - sec);

		ts.it_value.tv_sec = sec.count();
		ts.it_value.tv_nsec = nsec.count();
	}

	::timerfd_settime(timerFd, 0, &ts, NULL);
}

void TimerQueue::stopSystemTimer() {
	struct itimerspec ts{};
	::timerfd_settime(timerFd, 0, &ts, NULL);
}


TimerQueue::~TimerQueue() {
	close(timerFd);
}

