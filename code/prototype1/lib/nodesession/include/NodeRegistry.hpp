#pragma once
#include "IConnectionHandler.hpp"
#include "IDataAccessor.hpp"
#include <unordered_map>
#include <mutex>


class NodeRegistry {
	std::unordered_map<uint16_t, UserAuth> registry;
	std::mutex mut;
public: 
	bool isEmpty(UserAuth const& auth) const;

	UserAuth findSession(const uint16_t sessionId);
	UserAuth removeSession(const uint16_t sessionId);
	void addSession(const uint16_t sessionId, UserAuth const& user);
};
