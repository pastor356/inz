#include "ReactorFixture.hpp"
#include "ReactorNewFixture.hpp"

TEST_F(ReactorUTest, InitTest) {
	auto connMock = std::make_unique<StrictMock<ConnectionMock>>();

	EXPECT_CALL(*sockPtr, accept)
		.WillOnce(Return(std::move(connMock)));

	EXPECT_CALL(*(demux.get()), registerNode(_, _))
		.Times(1);

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

