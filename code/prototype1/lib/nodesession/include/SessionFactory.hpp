#pragma once
#include "IStateFactory.hpp"

class SessionFabric: public IStateFabric {
	std::shared_ptr<SessionRegistry> registry;
public:
	std::shared_ptr<IConnectionHandler> createSession(
			uint8_t qosLevel, 
			std::string const& password,
			std::string const& login,
			std::string const& secret,
			std::string const& topic) const override;

	std::shared_ptr<IConnectionHandler> createSession(SessionConfig& sc) const override;

	SessionFabric() = default;
};
