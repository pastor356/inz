#include "Session.hpp"
#include "Handshaker.hpp"
#include "DecoratorBuilder.hpp"
#include "MonitoredNode.hpp"
#include "InitialState.hpp"
#include "Serializer.hpp"

#include "QosMock.hpp"
#include "ConnectionMock.hpp"
#include "SocketMock.hpp"
#include "ContextMock.hpp"
#include "SessionFixture.hpp"
#include "PollMock.hpp"

#include "MessageRouter.hpp"
#include "AcceptorSocket.hpp"
#include "NodeInitializer.hpp"

#include "Reactor.hpp"
#include "Demultiplexer.hpp"
#include "Invoker.hpp"

using namespace ::testing;

class ReactorTest: public Test {
public:
	std::unique_ptr<StrictMock<ConnectionMock>> owner;
	ConnectionMock* connMock;

	std::unique_ptr<StrictMock<SocketMock>> sockOwner;
	SocketMock* sockMock;

	std::shared_ptr<IDataAccessor> dataAccessor;
	std::shared_ptr<IStateContext> monitoredNode;
	std::shared_ptr<IConnectionHandler> state;
	std::shared_ptr<NodeRegistry> nodeRegistry;

	std::shared_ptr<Reactor> reactor;
	std::shared_ptr<StrictMock<PollMock>> poller;
	std::shared_ptr<IInvoker> dispatcher;

	std::shared_ptr<EventHandler> messageRouter;
	std::shared_ptr<EventHandler> acceptorSocket;
	std::shared_ptr<EventHandler> nodeInitializer;

	ReactorTest() {
		owner = std::make_unique<StrictMock<ConnectionMock>>();
		connMock = owner.get();
		sockOwner = std::make_unique<StrictMock<SocketMock>>();
		sockMock = sockOwner.get();

		dataAccessor = std::make_shared<CacheAccessor>();
		nodeRegistry = std::make_shared<NodeRegistry>();
		state = std::make_shared<InitialState>(std::move(owner));
		monitoredNode = std::make_shared<MonitoredNode>(state, dataAccessor, nodeRegistry);
		state->setContext(monitoredNode);
		
		poller = std::make_shared<StrictMock<PollMock>>();
		dispatcher = std::make_shared<Invoker>();
		reactor = std::make_shared<Reactor>(poller, dispatcher);

		messageRouter = std::make_shared<MessageRouter>(reactor);
		nodeInitializer = std::make_shared<NodeInitializer>(reactor, messageRouter);
		//acceptorSocket = std::make_shared<AcceptorSocket>(std::move(sockOwner), 
				 //reactor, dataAccessor, nodeRegistry, nodeInitializer);
	}
};
