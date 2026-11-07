#pragma once
#include "EventHandler.hpp"
#include "IStateContext.hpp"
#include <map>
#include <sys/timerfd.h>



class TimerQueue: public IStateContext {
	std::multimap<timePoint, std::pair<timerId, callback>> timePointToCb;
	std::unordered_map<timerId, timePoint> idToTime;

	int timerFd{-1};
	timerId thisTimerId{3};
	
	void updateSystemTimer(timePoint tp);
	void stopSystemTimer();

	using clock = std::chrono::steady_clock;
	using timerId = uint64_t;
	using timePoint = clock::time_point;
	using callback = std::function<void()>;

public:

	TimerQueue();
	timerId addTimer(timePoint tp, callback cb);
	void cancelTimer(timerId const& tid);

	int getDescriptor() const override;
	IConnection::Status recv(Packet& packet, int flag = 0) override;
	IConnection::Status send(Packet& packet, int flag = 0) override;

	~TimerQueue();
};

