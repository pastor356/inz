#pragma once
#include "IConnectionHandler.hpp"
#include "IStateContext.hpp"

class AcceptorNode: public IStateContext {
	std::unique_ptr<ISocket> socket;
public:
	AcceptorNode(std::unique_ptr<ISocket> sock);
	std::unique_ptr<IConnection> accept() const;
	int getDescriptor() const override;

	void changeStateSession(std::unique_ptr<IConnection>, 
			std::shared_ptr<IStateContext>, UserAuth) override {
        throw std::logic_error("AcceptorNode: changeStateSession not supported");
    }

    void changeStateTerminated(std::shared_ptr<IStateContext>, UserAuth) override {
        throw std::logic_error("AcceptorNode: changeStateTerminated not supported");
    }

    IConnectionHandler* getHandler() const override { return nullptr; }

    void setRole(uint8_t) override { 
		throw std::logic_error("AcceptorNode: setRole not supported");
    }

    bool isConsumer() const override { 
		return false; 
	}

    IConnection::Status recv(Packet&, int) override { 
		throw std::logic_error("AcceptorNode: recv not supported"); 
	}

    IConnection::Status send(Packet&, int) override { 
		throw std::logic_error("AcceptorNode: send not supported"); 
	}

	void changeState(std::shared_ptr<IConnectionHandler> newState) override {}

	void changeStateHandshake(std::unique_ptr<IConnection> conn,
			std::shared_ptr<IStateContext> ctx) override {}

	void changeStateReconnect(std::unique_ptr<IConnection> conn, 
			std::shared_ptr<IStateContext> ctx) override {}

	bool isInitialized() const override { return(true); }
};
