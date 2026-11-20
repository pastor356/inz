#pragma once
#include <future>

class MethodRequest {
public:
	virtual bool canRun() const = 0;
	virtual void run() = 0;

	virtual ~MethodRequest() = default;
};
