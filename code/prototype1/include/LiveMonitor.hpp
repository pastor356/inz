#pragma once
#include <chrono>
#include "ConnectionDecorator.hpp"
#include "ConsoleLogger.hpp"

#ifndef MsgType_Pos
#define MsgType_Pos 0x4
#endif 

#define MSGTYPE_KEEPALIVE (uint8_t)(0x6 << MsgType_Pos)


class LiveMonitor: public ConnectionDecorator {
protected:
	std::chrono::milliseconds timeout;
	mutable std::chrono::steady_clock::time_point lastActivity;

	void resetTimer() const;
	bool isKeepAlive(Packet& packet) const;

public:
	using ConnectionDecorator::ConnectionDecorator;

	bool isSessionExpired() const;
	IConnection::Status recv(std::string& msg, int flag = 0) const override;
	IConnection::Status recv(Packet& packet, int flag = 0) const override;
};

