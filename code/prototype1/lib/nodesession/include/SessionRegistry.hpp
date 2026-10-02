#pragma once
#include <mutex>
#include "IConnectionHandler.hpp"
#include "Session.hpp"

class SessionRegistry {
	using SessionId = uint16_t;

	std::unordered_map<SessionId, std::shared_ptr<Session>> registry;
	std::mutex registryMutex;

public:
	std::shared_ptr<Session> findSession(SessionId si);
	void addSession(std::shared_ptr<Session> session);
};





