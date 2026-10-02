#include "SessionRegistry.hpp"

std::shared_ptr<Session> SessionRegistry::findSession(SessionId si) {
	const char* methodName = "SessionRegistry::findSession()";

	std::lock_guard<std::mutex> lock{registryMutex};

	auto session = registry.find(si);

	if(session == registry.end()) {
		logWarn << std::string{methodName} + ": session's not found";
		return(std::make_shared<Session>());
	} else {
		return(session->second);
	}
}



void SessionRegistry::addSession(std::shared_ptr<Session> session) {
	uint16_t id = session->getId();

	std::lock_guard<std::mutex> lock{registryMutex};
	registry.emplace(id, session);
}
