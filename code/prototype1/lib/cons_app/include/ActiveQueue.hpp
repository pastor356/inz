#pragma once
#include "MethodRequest.hpp"
#include <condition_variable>
#include <list>

class ActiveQueue {
	std::list<std::shared_ptr<MethodRequest>> activationList;

	std::mutex mtx;
	std::condition_variable blockEmpty;

public:
	ActiveQueue() = default;

	void insert(std::shared_ptr<MethodRequest> request);
	void remove(std::shared_ptr<MethodRequest> request);

	std::shared_ptr<MethodRequest> begin();
	std::shared_ptr<MethodRequest> end();
	bool empty() const;

};
