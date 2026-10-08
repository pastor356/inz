#include "CacheAccessor.hpp"

IDataAccessor::AccessorStatus CacheAccessor::authorize(UserAuth const& auth) const {
	const char* methodName = "CacheAcceptor::authorize(UserAuth const&)";
	auto entity = storage.find(auth.login);

	if(entity == storage.end()) {
		return(IDataAccessor::AccessorStatus::USER_NOT_FOUND);
	}
	
	if((auth.password == entity->second.password) && 
			(auth.secret == entity->second.secret)) {
		return(IDataAccessor::AccessorStatus::SUCCESS);
	}

	return(IDataAccessor::AccessorStatus::FAILURE);
}



IDataAccessor::AccessorStatus CacheAccessor::createUser(UserAuth const& auth) {
	const char* methodName = "CacheAcceptor::createUser(UserAuth const&)";
	auto res = storage.try_emplace(auth.login, auth);

	if(!res.second) {
		logError << std::string{methodName} + ": user's not inserted";
		return(IDataAccessor::AccessorStatus::USER_ALREADY_EXISTS);
	}

	return(IDataAccessor::AccessorStatus::SUCCESS);
}


void CacheAccessor::addTopic(std::string const& topic) {
	topics.insert(topic);
}


void CacheAccessor::getTopics(TopicSet& topicSet) const {
	topicSet = this->topics;
}
