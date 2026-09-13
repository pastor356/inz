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

TEST(QosMockTest, PendingMsgTest_OnSend) {
	auto owner = std::make_unique<StrictMock<ConnectionMock>>();
	std::string message = "dupa";

	auto* connMock = owner.get();

	EXPECT_CALL(*connMock, send(A<Packet&>(), 0))
		.WillOnce(Return(IConnection::Status::SUCCESS));

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

