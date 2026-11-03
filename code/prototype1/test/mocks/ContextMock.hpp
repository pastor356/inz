#pragma once
#include "IStateContext.hpp"
#include "MonitoredNode.hpp"
#include "IConnectionHandler.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class ContextMock: public IStateContext {
public:
	MOCK_METHOD(void, changeState, (std::shared_ptr<IConnectionHandler>));

	MOCK_METHOD(void, changeStateHandshake, 
			(std::unique_ptr<IConnection>, std::shared_ptr<IStateContext> ctx));

	MOCK_METHOD(void, changeStateReconnect, 
			(std::unique_ptr<IConnection>, std::shared_ptr<IStateContext> ctx));

	MOCK_METHOD(void, changeStateSession, 
			(std::unique_ptr<IConnection>, std::shared_ptr<IStateContext>, UserAuth));

	MOCK_METHOD(void, changeStateTerminated, 
			(std::shared_ptr<IStateContext>, UserAuth));

	MOCK_METHOD(void, setRole, (uint8_t), (override));

	MOCK_METHOD(bool, isConsumer, (), (const, override));

	MOCK_METHOD(int, getDescriptor, (), (const override));

	MOCK_METHOD(IConnectionHandler*, getHandler, (), (const override));

	MOCK_METHOD(IConnection::Status, recv, (Packet&, int), (override));
	MOCK_METHOD(IConnection::Status, send, (Packet&, int), (override));
	MOCK_METHOD(bool, isInitialized, (), (const, override));
};
