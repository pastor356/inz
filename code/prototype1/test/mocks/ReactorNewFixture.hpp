#pragma once
#include "IStateContext.hpp"
#include "ContextMock.hpp"

using namespace ::testing;

class ReactorUTest: public Test {
public:
	std::shared_ptr<StrictMock<IStateContext>> monitoredNode;
	std::unique_ptr<StrictMock<SocketMock>> sockMock;
	SocketMock* sockPtr;

	std::shared_ptr<IStateContext> acceptorNode;

	std::shared_ptr<IDataAccessor>		storage;
	std::shared_ptr<NodeRegistry>		registry;

	std::shared_ptr<EventHandler>		messageRouter;
	std::shared_ptr<EventHandler>		acceptorSocket;
	std::shared_ptr<EventHandler>		nodeInitializer;

	std::shared_ptr<Reactor>			reactor;
	std::shared_ptr<StrictMock<PollMock>>	demux;
	std::shared_ptr<IInvoker>			dispatcher;


	ReactorUTest() {
		sockMock = std::make_unique<StrictMock<SocketMock>>();
		sockPtr = sockMock.get();

		storage = std::make_shared<CacheAccessor>();
		registry = std::make_shared<NodeRegistry>();

		demux = std::make_shared<StrictMock<PollMock>>();
		dispatcher = std::make_shared<Invoker>();
		reactor = std::make_shared<Reactor>(demux, dispatcher);

		messageRouter = std::make_shared<MessageRouter>(reactor);
		nodeInitializer = std::make_shared<NodeInitializer>(reactor, messageRouter);
		acceptorNode = std::make_shared<AcceptorNode>(std::move(sockMock));

		acceptorSocket = std::make_shared<AcceptorSocket>(reactor, storage, 
				registry, nodeInitializer);

	}
};
