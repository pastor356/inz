#pragma once
#include <unordered_map>
#include <string>
#include <vector>

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

	AccessorStatus authorize(UserAuth const& auth) const = 0;
	AccessorStatus createUser(UserAuth const& auth) const = 0;
	void getTopics(TopicSet const& topicSet) const = 0;
	void addTopic(std::string const& topic) const = 0;
};


