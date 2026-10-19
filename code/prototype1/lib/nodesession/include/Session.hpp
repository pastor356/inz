#pragma once
#include "IConnectionHandler.hpp"
#include "IDataAccessor.hpp"

#define MSGTYPE_SESSIONMSG	(uint8_t)(0x8 << MsgType_Pos)
#define SESSIONHEADER_LEN	(uint8_t)11

enum class SessionState {
	DISCONNECTED,
	CONNECTED,
	EXPIRED,
	CONNECTING
};

class Session: public IConnectionHandler {
public:
	struct SessionHeader {
		uint16_t sessionId;
		std::chrono::nanoseconds timeStamp;
	};

private:
	static uint16_t sessionId; // incrementing in every constructor call
	std::unique_ptr<IConnection> connection;
	std::shared_ptr<IDataAccessor> storage; 

	uint16_t thisSessionId;
	UserAuth authData;
	SessionState state;

	uint8_t qosLevel;

	// timeOfCreation;
	std::weak_ptr<IStateContext> context;


	SessionHeader parseMsg(Packet& packet) const;
	void trimSessionHeader(Packet& packet) const;

	uint16_t getSessionIdReconnection(Packet const& packet) const;

public:

	// change to unique_ptr<IConnectionHandler>
	Session(std::unique_ptr<IConnection> connection, std::shared_ptr<IStateContext> ctx,
			UserAuth const& userAuth);

	void setConnection(std::unique_ptr<IConnection> conn) override;
	std::unique_ptr<IConnection> getConnection() override;
	void setContext(std::shared_ptr<IStateContext> ctx) override;

	int getDescriptor() const override;

	IConnection::Status send(std::string const& message, int flag = 0) const override;
	IConnection::Status recv(std::string& message, int flag = 0) override;

	IConnection::Status send(Packet& packet, int flag = 0) const override;
	IConnection::Status recv(Packet& packet, int flag = 0) override;
	
	// callback, injected into LiveMonitor (timeoutCallback field)
	void handleTimeout(); 

	uint8_t getQos() const;
	void setQos(uint8_t level);
	uint16_t getId() const; 
};

