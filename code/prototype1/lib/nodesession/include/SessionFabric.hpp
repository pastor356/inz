#pragma once
#include "IStateFabric.hpp"

class SessionFabric: public IStateFabric {
	std::shared_ptr<SessionRegistry> registry;
public:
	std::shared_ptr<Session> createSession(
			uint8_t qosLevel, 
			std::string const& password,
			std::string const& login,
			std::string const& secret,
			std::string const& topic) const override;

	SessionFabric() = default;
};




