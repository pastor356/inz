#pragma once
#include <thread>
#include "MethodRequest.hpp"
#include "ActiveQueue.hpp"


class Scheduler {
	std::unique_ptr<ActiveQueue> activeQueue;

	static void* (*func)(void* arg);

	std::thread thr;

public:
	Scheduler(std::unique_ptr<ActiveQueue> que); 
	void insert(std::shared_ptr<MethodRequest> request);

	virtual void dispatch(); 

	virtual ~Scheduler(); 
};
