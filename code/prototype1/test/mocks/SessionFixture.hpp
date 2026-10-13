#include "MonitoredNode.hpp"
#include "CacheAccessor.hpp"
#include "NodeRegistry.hpp"
#include "InitialState.hpp"
#include "Handshaker.hpp"
#include "ReconnectState.hpp"
#include "ConnectionMock.hpp"
#include "Session.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>


using namespace ::testing;

Packet serializeHandshakePacket(bool role, uint8_t qos, bool creat, bool newUsr,
				uint8_t rights, std::string topic, std::string login,
				std::string password, std::string secret);

class SessionTest: public Test {
public:
	std::unique_ptr<StrictMock<ConnectionMock>> owner;
	ConnectionMock* connMock;
	std::shared_ptr<IDataAccessor> dataAccessor;
	std::shared_ptr<IStateContext> monitoredNode;
	std::shared_ptr<IConnectionHandler> state;
	std::shared_ptr<NodeRegistry> nodeRegistry;

	SessionTest() {
		owner = std::make_unique<StrictMock<ConnectionMock>>();
		connMock = owner.get();


		dataAccessor = std::make_shared<CacheAccessor>();
		nodeRegistry = std::make_shared<NodeRegistry>();

		state = std::make_shared<InitialState>(std::move(owner));
		monitoredNode = std::make_shared<MonitoredNode>(state, dataAccessor, nodeRegistry);

		state->setContext(monitoredNode);
	}
};
