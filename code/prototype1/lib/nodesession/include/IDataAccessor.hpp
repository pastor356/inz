#pragma once
#include <unordered_map>
#include <string>
#include <set>

struct UserAuth {
	std::string login;
	std::string password;
	std::string secret;
	uint8_t rights;
};


class IDataAccessor {
public:
	enum class AccessorStatus: uint8_t {
		SUCCESS = 0,
		FAILURE,
		USER_NOT_FOUND,
		USER_ALREADY_EXISTS,
		INVALID_PASSWORD,
		INVALID_SECRET
	};

	using TopicSet = std::set<std::string>;

	virtual AccessorStatus authorize(UserAuth const& auth) const = 0;
	virtual AccessorStatus createUser(UserAuth const& auth) = 0;
	virtual void getTopics(TopicSet& topicSet) const = 0;
	virtual void addTopic(std::string const& topic) = 0;

	virtual ~IDataAccessor() = default;
};
