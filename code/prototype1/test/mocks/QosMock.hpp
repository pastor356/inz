#pragma once
#include "TcpConnection.hpp"
#include "TcpSocket.hpp"
#include "ConsoleLogger.hpp"
#include "SocketMock.hpp"
#include "Qos1.hpp"
#include "LiveMonitor.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>


class TestableQos: public Qos1 {
public:
	using Qos1::Qos1;

	using PendingMessage = IQualityStrategy::PendingMessage;
	using MsgType =  IQualityStrategy::MsgType;
	using QosState = IQualityStrategy::QosState;

	uint16_t currentId() { return(this->currMsgId); }

	std::unordered_map<uint16_t, PendingMessage>& pending() { return(this->pendingMessages); }

	void insertPending(uint16_t id, PendingMessage pending) {
		this->pendingMessages[id] = std::move(pending);
	}

	std::shared_ptr<PendingMessage> parse(Packet const& packet) {
		return(parseMsg(packet));
	}
};
