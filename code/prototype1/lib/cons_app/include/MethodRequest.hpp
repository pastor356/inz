#pragma once


class MethodRequest {
	virtual bool canRun() const = 0;
	virtual void call() = 0;
};

