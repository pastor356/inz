#pragma once
#include "IConnectionHandler.hpp"

enum class SessionState {
	DISCONNECTED,
	CONNECTED,
	EXPIRED,
	CONNECTING
};

class Session: public IConnectionHandler {
	static uint16_t sessionId; // incrementing in every constructor call
	std::unique_ptr<IConnection> connection;
	uint16_t thisSessionId;
	std::string password;
	std::string secret;
	SessionState state;

	// timeOfCreation;
	std::weak_ptr<IStateContext> context;

	struct SessionHeader {
		uint16_t sessionId;
		std::chrono::milliseconds timeStamp;
	};

	SessionHeader parseMsg(Packet& packet) const;
	uint16_t getSessionIdReconnection(Packet const& packet) const;

public:

	Session(std::unique_ptr<IConnection> conn, std::shared_ptr<IStateContext> ctx,
			std::string const& password, std::string const& secrt);

	void setConnection(std::unique_ptr<IConnection> conn);

	IConnection::Status send(std::string const& message, int flag = 0) const override;
	IConnection::Status recv(std::string& message, int flag = 0) const override;

	IConnection::Status send(Packet& packet, int flag = 0) const override;
	IConnection::Status recv(Packet& packet, int flag = 0) const override;
	//void setContext(std::shared_ptr<IStateContext> ctx) override; 

	
	// callback, injected into LiveMonitor (timeoutCallback field)
	void handleTimeout(); 
};

