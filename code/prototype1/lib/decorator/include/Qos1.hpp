#pragma once
#include "IQualityStrategy.hpp"
#include <gtest/gtest.h>


class Qos1: public IQualityStrategy {
protected:

	IConnection::Status sendPubAck(uint16_t packetId) const override;
	IConnection::Status sendPubRec(uint16_t packetId) const override;
	IConnection::Status sendPubRel(uint16_t packetId) const override;
	IConnection::Status sendPubComp(uint16_t packetId) const override;

	void trimQosHeader(std::vector<uint8_t>& header) const override;
public:
	std::shared_ptr<IQualityStrategy::PendingMessage> 
		parseMsg(Packet const& packet) const override;  // PRIVATE
	using IQualityStrategy::IQualityStrategy;

	IConnection::Status send(Packet& packet, int flag = 0) const override;
	IConnection::Status recv(Packet& packet, int flag = 0) const override;

	IConnection::Status send(std::string const& msg, int flag = 0) const override;
	IConnection::Status recv(std::string& msg, int flag = 0) const override;
};
