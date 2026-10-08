#include "NodeRegistry.hpp"


UserAuth NodeRegistry::findSession(const uint16_t sessionId) {
	const char* methodName = "NodeRegistry::findSession(const uint16_t)";
	
	std::unique_lock<std::mutex> lock(mut);
	auto session = registry.find(sessionId);
	UserAuth res;

	if(session == registry.end()) {
		logError << std::string{methodName} + ": session's not found";
		return(res);
	}

	res = session->second;
	return(res);
}

void NodeRegistry::addSession(const uint16_t sessionId, UserAuth const& user) {
	const char* methodName = "NodeRegistry::addSession(const uint16_t)";

	std::unique_lock<std::mutex> lock(mut);
	auto res = registry.try_emplace(sessionId, user);

	if(!res.second) {
		logError << std::string{methodName} + ": session already exists";
		return;
	}
}


UserAuth NodeRegistry::removeSession(const uint16_t sessionId) {
	const char* methodName = "NodeRegistry::removeSession(const uint16_t)";
	std::unique_lock<std::mutex> lock(mut);
	auto session = registry.find(sessionId);

	auto ret = session->second;

	if(session != registry.end()) {
		registry.erase(session);
	} 
	else logError << std::string{methodName} + ": session's not found";

	return(ret);
}

bool NodeRegistry::isEmpty(UserAuth const& auth) const {
	if((auth.login.empty() || auth.password.empty()) && auth.secret.empty()) {
		return(true);
	}

	return(false);
}
