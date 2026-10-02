#pragma once
#include "IDataAccessor.hpp"


class CacheAccessor: public IDataAccessor {
	std::unordered_map<std::string, UserAuth> storage;
	TopicSet topics;
public:
	IDataAccessor::AccessorStatus authorize(UserAuth const& auth) const override;

	IDataAccessor::AccessorStatus createUser(UserAuth const& auth) const override;

	void getTopics(TopicSet& topicSet) const override;

	void addTopic(std::string const& topic) const override;
};

