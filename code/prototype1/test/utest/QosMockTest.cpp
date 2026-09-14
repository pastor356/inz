#include "TcpConnection.hpp"
#include "TcpSocket.hpp"
#include "ConsoleLogger.hpp"
#include "SocketMock.hpp"
#include "Qos1.hpp"
#include "LiveMonitor.hpp"

#include "QosMock.hpp"
#include "ConnectionMock.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace ::testing;

/* 
 * checking if after calling send(packet) packet is appearing 
 * in pendingMessages map
 */
TEST(QosMockTest, PendingMsgTest_OnSend) {
	auto owner = std::make_unique<StrictMock<ConnectionMock>>();
	std::string message = "dupa";

	auto* connMock = owner.get();

	EXPECT_CALL(*connMock, send(A<Packet&>(), 0))
		.WillOnce(
			[=](Packet& packet, int flag) {
				(void) packet; 
				(void) flag;
				return(IConnection::Status::SUCCESS);
			}
		);

	TestableQos qos(std::move(owner));

	Packet packet;
	packet.header.resize(9, 0);
	packet.payload = message;

	uint16_t expectedId = qos.currentId(); // id before incrementing inside Qos1::send()
	EXPECT_EQ(qos.send(packet), IConnection::Status::SUCCESS);

	const auto& map = qos.pending();

	const auto& pending = map.find(expectedId);

	ASSERT_NE(pending, map.end()); // preventing null dereferencing

	TestableQos::PendingMessage pm = pending->second;

	EXPECT_EQ(pm.packetId, expectedId);
    EXPECT_EQ(pm.msgType, TestableQos::MsgType::DATA);
    EXPECT_EQ(pm.state, TestableQos::QosState::WAIT_PUBACK);
    EXPECT_EQ(pm.payload, message);
    EXPECT_EQ(pm.retries, 0);
};



/*
 * checking if after calling recv(packet) with puback
 * pendingMessages map is empty
 */
TEST(QosMockTest, PendingMsgTest_Puback) {
	auto owner = std::make_unique<StrictMock<ConnectionMock>>();

	StrictMock<ConnectionMock>* mock = owner.get();

	TestableQos qos{std::move(owner)};

	Packet packet; // incoming packet
	packet.header.reserve(3);
	packet.header.push_back(MSGTYPE_PUBACK);
	packet.header.push_back(qos.currentId() & 0xff);
	packet.header.push_back((qos.currentId() >> 8) & 0xff);

	
	EXPECT_CALL(*mock, recv(A<Packet&>(), 0))
		.WillOnce(DoAll(SetArgReferee<0>(packet), Return(IConnection::Status::SUCCESS)));


	// packet with data, waits for PubAck (inside container)
	TestableQos::PendingMessage pending_msg;
	pending_msg.packetId = qos.currentId();
	pending_msg.msgType = TestableQos::MsgType::DATA;
	pending_msg.payload = "dupa";
	pending_msg.state = TestableQos::QosState::WAIT_PUBACK;

	qos.insertPending(qos.currentId(), pending_msg); 
	ASSERT_EQ(1u, qos.pending().size()); // one packet in container
	
	Packet incoming_pack;
	EXPECT_EQ(IConnection::Status::SUCCESS, qos.recv(incoming_pack));

	auto parsed_msg = qos.parse(incoming_pack);

	ASSERT_EQ(incoming_pack.header.size(), QOSHEADER_LEN);
	EXPECT_EQ(parsed_msg->packetId, qos.currentId());
	EXPECT_EQ(parsed_msg->msgType, TestableQos::MsgType::PUBACK);
	EXPECT_EQ(parsed_msg->state, TestableQos::QosState::FINISHED);

	
	ASSERT_EQ(0u, qos.pending().size()); // container should be empty after 
										 // getting puback
}
	


