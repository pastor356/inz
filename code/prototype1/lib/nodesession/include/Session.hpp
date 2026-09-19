#pragma once
#include "IConnectionHandler.hpp"


class Session: public IConnectionHandler {
	static uint16_t sessionId; // incrementing in every constructor call
	std::unique_ptr<IConnection> connection;
	uint16_t thisSessionId;
	std::string password;
	std::string secret;
	SessionState state;
	// timeOfCreation;

public:
	enum class SessionState {
		DISCONNECTED,
		CONNECTED,
		EXPIRED,
		CONNECTING
	};

	Session(std::unique_ptr<IConnection> conn, std::string& password, std::string& secret);

	IConnection::Status send(std::string const& message, int flag = 0) const override;
	IConnection::Status recv(std::string& message, int flag = 0) const override;
	void setCtx(std::shared_ptr<INodeCtx> ctx) override; 
	
	// callback, injected into LiveMonitor (timeoutCallback field)
	void handleTimeout(); 
};

