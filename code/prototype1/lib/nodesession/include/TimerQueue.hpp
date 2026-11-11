#pragma once
#include <map>
#include <sys/timerfd.h>

#include "IStateContext.hpp"
#include "ITimerQueue.hpp"


class TimerQueue: public IStateContext, public ITimerQueue {
	std::multimap<ITimerQueue::timePoint, 
		std::pair<ITimerQueue::timerId, ITimerQueue::callback>> timePointToCb;

	std::unordered_map<ITimerQueue::timerId, ITimerQueue::timePoint> idToTime;

	int timerFd{-1};
	ITimerQueue::timerId thisTimerId{3};
	
	void updateSystemTimer(ITimerQueue::timePoint tp);
	void stopSystemTimer();

public:
	TimerQueue();

	ITimerQueue::timerId addTimer(ITimerQueue::timePoint tp, ITimerQueue::callback cb) override;
	void cancelTimer(ITimerQueue::timerId const& tid) override;

	int getDescriptor() const override;
	IConnection::Status recv(Packet& packet, int flag = 0) override;
	IConnection::Status send(Packet& packet, int flag = 0) override;

	~TimerQueue() override;



	void changeState(std::shared_ptr<IConnectionHandler> newState) override {
		(void)newState;
	}

	void changeStateHandshake(std::unique_ptr<IConnection> conn,
							  std::shared_ptr<IStateContext> ctx) override {
		(void)conn;
		(void)ctx;
	}

	void changeStateReconnect(std::unique_ptr<IConnection> conn, 
			std::shared_ptr<IStateContext> ctx) override {
		(void)conn;
		(void)ctx;
	}

	void changeStateSession(std::unique_ptr<IConnection> conn,
							std::shared_ptr<IStateContext> ctx,
							UserAuth authUser) override {
		(void)conn;
		(void)ctx;
		(void)authUser;
	}

	void changeStateTerminated(std::shared_ptr<IStateContext> ctx,
							   UserAuth authUser) override {
		(void)ctx;
		(void)authUser;
	}

	IConnectionHandler* getHandler() const override {
		return nullptr;
	}

	void setRole(uint8_t role) override {
		(void)role;
	}

	bool isConsumer() const override {
		return false;
	}

	bool isInitialized() const override {
		return false;
	}
};
