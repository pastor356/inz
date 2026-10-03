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

	auto dataAccessor = std::make_shared<CacheAccessor>();
	auto nodeRegistry = std::make_shared<NodeRegistry>();

	auto initState = std::make_shared<InitialState>(std::move(owner));
	auto monitoredNode = std::make_shared<MonitoredNode>(initState, dataAccessor, nodeRegistry);
}

