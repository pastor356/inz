#pragma once
#include "IDataAccessor.hpp"


class CacheAccessor: public IDataAccessor {
	std::unordered_map<std::string, UserAuth> storage; // login -> UserAuth
	TopicSet topics;

public:
	CacheAccessor() = default;
	UserAuth getUser(std::string const& login) const override;
	IDataAccessor::AccessorStatus authorize(UserAuth const& auth) const override;
	IDataAccessor::AccessorStatus createUser(UserAuth const& auth) override;
	void getTopics(TopicSet& topicSet) const override;
	void addTopic(std::string const& topic) override;
};

