#pragma once
#include <chrono>
#include "ConnectionDecorator.hpp"


class LiveMonitor: public ConnectionDecorator {
protected:
	std::chrono::milliseconds timeout;
	mutable std::chrono::steady_clock::time_point lastActivity;

	void resetTimer() const;
	bool isKeepAlive(std::string const& msg) const;

public:
	bool isSessionExpired() const;
	IConnection::Status recv(std::string& msg, int flag = 0) const override;
};

