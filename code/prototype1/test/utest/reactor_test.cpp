#include "ReactorFixture.hpp"
#include "ReactorNewFixture.hpp"


TEST_F(ReactorUTest, InitTest) {
	auto connMock = std::make_unique<StrictMock<ConnectionMock>>();

	EXPECT_CALL(*sockPtr, accept())
		.WillOnce(Return(std::move(connMock)));

	EXPECT_CALL(*(demux.get()), registerNode(_, _))
		.Times(2);

	reactor->registerNode(acceptorNode, acceptorSocket);

	std::vector<IPoll::ReadyEvent> retVec;
	IPoll::ReadyEvent event;
	auto eventEnt = std::make_shared<IPoll::EventEntity>();
	eventEnt->node = acceptorNode;
	eventEnt->handler = acceptorSocket;

	event.entity = eventEnt;
	event.flags = EPOLLIN;

	retVec.push_back(event);

	EXPECT_CALL(*(demux.get()), wait)
		.WillOnce(Return(retVec));

	reactor->runOnce();
}




TEST_F(ReactorUTest, NodeInitializerTest) {

	std::shared_ptr<StrictMock<ContextMock>> monitoredNode = 
		std::make_shared<StrictMock<ContextMock>>();

	EXPECT_CALL(*(demux.get()), registerNode(_, _)).Times(2);
	EXPECT_CALL(*(demux.get()), deregisterNode(_)).Times(1);

	EXPECT_CALL(*(monitoredNode.get()), getDescriptor())
		.WillOnce(Return(3));

	EXPECT_CALL(*(monitoredNode.get()), recv(A<Packet&>(), 0))
		.Times(2).WillOnce(Return(IConnection::Status::SUCCESS))
				.WillOnce(Return(IConnection::Status::SUCCESS));


	reactor->registerNode(monitoredNode, nodeInitializer);

	std::vector<IPoll::ReadyEvent> retVec;
	auto eventEnt = std::make_shared<IPoll::EventEntity>(
			IPoll::EventEntity{monitoredNode, nodeInitializer, monitoredNode->getDescriptor()});

	IPoll::ReadyEvent event{eventEnt, EPOLLIN};

	retVec.push_back(event);

	EXPECT_CALL(*(demux.get()), wait()).Times(2)
		.WillOnce(Return(retVec))
		.WillOnce(Return(retVec));

	reactor->runOnce();
	reactor->runOnce();
}



TEST_F(ReactorUTest, InitSequenceTest) {
	std::shared_ptr<StrictMock<ContextMock>> monitoredNode = 
		std::make_shared<StrictMock<ContextMock>>();

	EXPECT_CALL(*(demux.get()), registerNode(_, _)).Times(2);
	EXPECT_CALL(*(demux.get()), deregisterNode(_)).Times(1);

	reactor->registerNode(monitoredNode, nodeInitializer);

	EXPECT_CALL(*(monitoredNode.get()), getDescriptor()).Times(2)
		.WillOnce(Return(3))
		.WillOnce(Return(3));

	/*
	 * 1) IConnectionHandler == InitialState -> Handshaker
	 * 2) IConnectionHandler == Handshaker -> Session
	 */
	EXPECT_CALL(*(monitoredNode.get()), recv(A<Packet&>(), 0)).Times(2)
			.WillOnce(Return(IConnection::Status::SUCCESS))
			.WillOnce(Return(IConnection::Status::SUCCESS));

	auto ent = std::make_shared<IPoll::EventEntity>(
			IPoll::EventEntity{monitoredNode, 
						nodeInitializer, 
						monitoredNode->getDescriptor()});

	IPoll::ReadyEvent re = IPoll::ReadyEvent{ent, EPOLLIN};
	std::vector<IPoll::ReadyEvent> vec;
	vec.push_back(re);

	/*
	 * 1)  IConnectionHandler == InitialState -> Handshaker
	 * 2)  IConnectionHandler == Handshaker -> Session
	 */
	EXPECT_CALL(*(demux.get()), wait()).Times(2)
		.WillOnce(Return(vec))
		.WillOnce(Return(vec));

	reactor->runOnce();
	reactor->runOnce();

	vec.clear();

	auto newEnt = std::make_shared<IPoll::EventEntity>(
			IPoll::EventEntity{monitoredNode, 
						messageRouter, 
						monitoredNode->getDescriptor()});

	IPoll::ReadyEvent newRe{newEnt, EPOLLIN};
	vec.push_back(newRe);
}



TEST_F(ReactorUTest, InitSequence2Test) {
	auto connMock = std::make_unique<StrictMock<ConnectionMock>>();

	EXPECT_CALL(*sockPtr, accept())
		.WillOnce(Return(std::move(connMock)));

	EXPECT_CALL(*(demux.get()), registerNode(_, _))
		.Times(2);

	reactor->registerNode(acceptorNode, acceptorSocket);

	std::vector<IPoll::ReadyEvent> retVec;

	auto eventEnt = std::make_shared<IPoll::EventEntity>(
			IPoll::EventEntity{acceptorNode, acceptorSocket, 
			acceptorNode->getDescriptor()});

	IPoll::ReadyEvent event{eventEnt, EPOLLIN};

	EXPECT_CALL(*(demux.get()), wait()).Times(2)
		.WillOnce(Return(retVec))
		.WillOnce(Return(retVec));

	reactor->runOnce();
	reactor->runOnce();
}




TEST_F(ReactorTest, IntegrationTest) {
	auto acceptedOwner = std::make_unique<StrictMock<ConnectionMock>>();
    auto* acceptedConn = acceptedOwner.get();

	int sockDesc = 66;
	EXPECT_CALL(*sockMock, getDescriptor()).Times(5)
		.WillRepeatedly(Return(sockDesc));

	EXPECT_CALL(*waiterPtr, ctl(_, _, _, _))
		.WillOnce(Return(0));

	reactor->registerNode(acceptorNode, acceptorSocket);

	struct epoll_event fake_events[1];
	fake_events[0].events = EPOLLIN;
	fake_events[0].data.fd = sockDesc;

	EXPECT_CALL(*waiterPtr, wait(_, _, _, _))
		.WillOnce(DoAll(
					SetArrayArgument<1>(fake_events, fake_events + 1),
					Return(1)));

    EXPECT_CALL(*sockMock, accept())
        .WillOnce(Return(std::move(acceptedOwner)));

	EXPECT_CALL(*waiterPtr, ctl(_, _, _, _))
		.WillOnce(Return(0));

	EXPECT_CALL(*acceptedConn, getDescriptor()).Times(5)
		.WillRepeatedly(Return(122));

	reactor->runOnce();
	reactor->runOnce();
}


