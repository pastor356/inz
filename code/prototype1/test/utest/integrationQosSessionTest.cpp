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


TEST_F(SessionTest, TransitionToSession) {
	monitoredNode->changeStateHandshake(state->getConnection(), monitoredNode);

	Packet packet = Serializer::serializeHandshake(
			true,		// role
			1,			// qos
			true,		// creat
			true,		// newUsr
			RIGHTS_USR, // rights
			"topic1", 
			"login",
			"password",
			"secret"
	);

	Packet sendPack;
	EXPECT_CALL(*connMock, send(A<Packet&>(), 0))
		.WillOnce(DoAll(SaveArg<0>(&sendPack), // actual message
					Return(IConnection::Status::SUCCESS)))
		.WillOnce(Return(IConnection::Status::SUCCESS)); // ack in Handshaker::recv()
	
	monitoredNode->send(packet, 0);

	EXPECT_CALL(*connMock, recv(A<Packet&>(), 0))
		.WillOnce(DoAll(SetArgReferee<0>(packet),
				Return(IConnection::Status::SUCCESS)));

	Packet incoming;
	monitoredNode->recv(incoming, 0);
	auto* handler = dynamic_cast<Session*>(monitoredNode->getHandler());
	EXPECT_NE(handler, nullptr);

	auto nodeAuth = nodeRegistry->findSession(300);

	EXPECT_EQ(nodeAuth.login, std::string{"login"});
}


TEST_F(SessionTest, TransitionToTerminated) {
	UserAuth auth;
	auth.login = std::move(std::string{"login"});
	auth.password = std::move(std::string{"password"});
	auth.secret = std::move(std::string{"secret"});
	auth.rights = RIGHTS_USR;

	monitoredNode->changeStateSession(state->getConnection(), monitoredNode, auth);

	EXPECT_CALL(*connMock, send(A<Packet&>(), 0))
		.WillOnce(Return(IConnection::Status::ERROR));

	Packet dummy;
	dummy.header.push_back(33);
	monitoredNode->send(dummy, 0);

	auto* handler = dynamic_cast<TerminatedState*>(monitoredNode->getHandler());

	EXPECT_NE(handler, nullptr);

	auto nodeAuth = nodeRegistry->findSession(300);
	EXPECT_EQ(nodeAuth.login, std::string{"login"});
	EXPECT_EQ(nodeAuth.password, std::string{"password"});
	EXPECT_EQ(nodeAuth.secret, std::string{"secret"});
}


TEST_F(SessionTest, TransitionTerminatedToSession) {
	UserAuth auth;
	auth.login = std::move(std::string{"login"});
	auth.password = std::move(std::string{"password"});
	auth.secret = std::move(std::string{"secret"});
	auth.rights = RIGHTS_USR;
	nodeRegistry->addSession(300, auth);

	monitoredNode->changeStateTerminated(monitoredNode, auth);

	Packet reconnectPack = Serializer::serializeReconnect(300);
	EXPECT_CALL(*connMock, recv(A<Packet&>(), 0))
		.WillOnce(DoAll(SetArgReferee<0>(reconnectPack),
				Return(IConnection::Status::SUCCESS)));


	(monitoredNode->getHandler())->setConnection(state->getConnection());

	auto* handler = dynamic_cast<ReconnectState*>(monitoredNode->getHandler());
	EXPECT_NE(handler, nullptr);

	Packet pack;
	monitoredNode->recv(pack, 0);

	auto* Sesshandler = dynamic_cast<Session*>(monitoredNode->getHandler());
	EXPECT_NE(Sesshandler, nullptr);
}



TEST_F(SessionTest, TransitionInitToSession) {
	Packet incoming;
	Packet init = Serializer::serializeInitial();
	Packet handshake = Serializer::serializeHandshake(
			true,
			1,
			true,
			true,
			RIGHTS_USR,
			"topic",
			"login",
			"password",
			"secret"
	);

	EXPECT_CALL(*connMock, recv(A<Packet&>(), 0))
		.WillOnce(DoAll(SetArgReferee<0>(init),
					Return(IConnection::Status::SUCCESS)))
		.WillOnce(DoAll(SetArgReferee<0>(handshake),
					Return(IConnection::Status::SUCCESS)));

	EXPECT_CALL(*connMock, send(A<Packet&>(), 0))
		.WillOnce(Return(IConnection::Status::SUCCESS));

	monitoredNode->recv(incoming, 0);

	auto* handshakeHandler = dynamic_cast<Handshaker*>(monitoredNode->getHandler());
	EXPECT_NE(handshakeHandler, nullptr);

	monitoredNode->recv(incoming, 0);

	auto* sessionHandler = dynamic_cast<Session*>(monitoredNode->getHandler());
	EXPECT_NE(sessionHandler, nullptr);

	auto newSessionAuth = nodeRegistry->findSession(300);
	EXPECT_EQ(newSessionAuth.login, "login");
	EXPECT_EQ(newSessionAuth.password, "password");
	EXPECT_EQ(newSessionAuth.secret, "secret");
	EXPECT_EQ(newSessionAuth.rights, RIGHTS_USR);

	auto node = dataAccessor->getUser(newSessionAuth.login);
	EXPECT_EQ(node.login, "login");
	EXPECT_EQ(node.password, "password");
	EXPECT_EQ(node.secret, "secret");
	EXPECT_EQ(node.rights, RIGHTS_USR);
}
