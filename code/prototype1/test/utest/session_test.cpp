#include "Session.hpp"
#include "Handshaker.hpp"
#include "DecoratorBuilder.hpp"
#include "MonitoredNode.hpp"

#include "QosMock.hpp"
#include "ConnectionMock.hpp"

using namespace ::testing;


TEST(SesionTest, StateChanging) {
	auto owner = std::make_unique<StrictMock<ConnectionMock>>();
	auto* connMock = owner.get();

	auto monitoredNode = std::make_shared<MonitoredNode>();

	auto handshaker = std::make_shared<Handshaker>(std::move(owner), monitoredNode);
	Packet handshake_pkt = handshaker->serializeHandshakePacket(
        /*role=*/true,
        /*qos=*/1,
        /*creat=*/true,
        /*newUsr=*/true,
        RIGHTS_USR,
        "topic1",
        "login",
        "passwd",
        "secret"
    );

	monitoredNode->changeState(handshaker);

	EXPECT_CALL(*connMock, recv(A<Packet&>(), 0))
		.WillOnce(DoAll(SetArgReferee<0>(handshake_pkt), 
					Return(IConnection::Status::SUCCESS)));

    Packet incomingPacket;
    IConnection::Status status = handshaker->recv(incomingPacket, 0);

    EXPECT_EQ(status, IConnection::Status::SUCCESS);

    auto currentHandler = monitoredNode->getHandler();
    EXPECT_NE(std::dynamic_pointer_cast<Session>(currentHandler), nullptr);
}





