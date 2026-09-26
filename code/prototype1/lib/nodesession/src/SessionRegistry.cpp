#include "SessionRegistry.hpp"

std::shared_ptr<Session> SessionRegistry::findSession(uint16_t si) const {
	const char* methodName = "SessionRegistry::findSession()";

	std::lock_guard<std::mutex> lock{registryMutex};

	auto session = registry.find(si);

	if(session == registry.end()) {
		logWarn << methodName + ": session's not found";
		return(std::make_shared_ptr<Session>{nullptr});
	} else {
		return(std::make_shared<Session>{session});
	}
}



void SessionRegistry::addSession(std::shared_ptr<Session> session) {
	uint16_t id = session->getId();

	std::lock_guard<std::mutex> lock{registryMutex};
	registry.emplace(id, session);
}
