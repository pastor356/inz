#pragma once
#include "MethodRequest.hpp"

class ActiveQueue {
	std::list<std::shared_ptr<MethodRequest>> activationList;
public:
	void insert(std::shared_ptr<MethodRequest> request, int timeout = -1);
	void remove(std::shared_ptr<MethodRequest> request, int timeout = -1);

};
