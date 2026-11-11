#pragma once
#include <chrono>
#include <functional>

class ITimerQueue {
public:
	using clock = std::chrono::steady_clock;
	using timerId = uint64_t;
	using timePoint = clock::time_point;
	using callback = std::function<void()>;

	virtual timerId addTimer(timePoint tp, callback cb) = 0;
	virtual void cancelTimer(timerId const& tid) = 0;

	virtual ~ITimerQueue() = default; 
};

