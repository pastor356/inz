#pragma once
#include "MethodRequest.hpp"
#include "ActiveQueue.hpp"

class Scheduler {
	std::unique_ptr<ActiveQueue> activeQueue;
public:
	void insert(std::shared_ptr<MethodRequest> request);
	virtual void dispatch(); 
};



