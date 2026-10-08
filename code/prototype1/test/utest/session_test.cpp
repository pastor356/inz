#include "Session.hpp"
#include "Handshaker.hpp"
#include "DecoratorBuilder.hpp"
#include "MonitoredNode.hpp"
#include "InitialState.hpp"
#include "Serializer.hpp"

#include "QosMock.hpp"
#include "ConnectionMock.hpp"
#include "SessionFixture.hpp"
#include "ContextMock.hpp"


using namespace ::testing;

TEST_F(SessionTest, InitState) {

	Packet packet; packet.header.push_back(MSGTYPE_HANDSHAKE);
	EXPECT_CALL(*connMock, recv(A<Packet&>(), 0))
		.WillOnce(DoAll(
					SetArgReferee<0>(packet), 
					Return(IConnection::Status::SUCCESS)));


	Packet incoming;
	auto status = state->recv(incoming, 0);

	EXPECT_EQ(status, IConnection::Status::SUCCESS);
}


TEST_F(SessionTest, HandshakePacketHandling) {

	Packet incoming;
	Packet initPacket;
	initPacket.header.push_back(MSGTYPE_HANDSHAKE);


	EXPECT_CALL(*connMock, recv(A<Packet&>(), 0))
		.WillOnce(DoAll(
					SetArgReferee<0>(initPacket), 
					Return(IConnection::Status::SUCCESS)));

	EXPECT_CALL(*connMock, send(A<Packet&>(), 0))
		.WillOnce(Return(IConnection::Status::SUCCESS));

	auto status = monitoredNode->recv(incoming, 0);
	EXPECT_EQ(status, IConnection::Status::SUCCESS);


	Packet handshakePacket;
	handshakePacket = Serializer::serializeHandshake(true, 1, true, true,  
		1, "topic1", "login", "password", "secret");


	EXPECT_CALL(*connMock, recv(A<Packet&>(), 0))
		.WillOnce(DoAll(
					SetArgReferee<0>(handshakePacket), 
					Return(IConnection::Status::SUCCESS)));

	status = monitoredNode->recv(incoming, 0);
	EXPECT_EQ(status, IConnection::Status::SUCCESS);

	// check if context has Handshaker state
	auto* handler = dynamic_cast<Handshaker*>(monitoredNode->getHandler());
	EXPECT_NE(handler, nullptr);
	
}

