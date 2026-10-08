#include "Session.hpp"
#include "Handshaker.hpp"
#include "DecoratorBuilder.hpp"
#include "MonitoredNode.hpp"
#include "InitialState.hpp"
#include "Serializer.hpp"

#include "QosMock.hpp"
#include "ConnectionMock.hpp"
#include "ContextMock.hpp"
#include "SessionFixture.hpp"


TEST_F(SessionTest, QosSessionIntegrTest) {
	std::unique_ptr<IConnection> qosConn = 
		std::make_unique<Qos1>(std::move(owner));


	monitoredNode->changeStateHandshake(std::move(qosConn), monitoredNode);

	Packet packet = Serializer::serializeHandshake(
			true,	// role
			1,		// qos
			true,	// creat
			true,	// newUsr
			RIGHTS_USR, // rights
			"topic1", 
			"login",
			"password",
			"secret"
	);

	EXPECT_CALL(*connMock, recv(A<Packet&>(), 0))
		.WillOnce(DoAll(SetArgReferee<0>(packet), 
					Return(IConnection::Status::SUCCESS)));

	Packet incoming;
	monitoredNode->recv(incoming, 0);

}
	










	

