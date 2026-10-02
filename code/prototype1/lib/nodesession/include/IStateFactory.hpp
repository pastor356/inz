#pragma once
#include "IConnectionHandler.hpp"
#include "Handshaker.hpp"
#include "SessionConfig.hpp"

class IStateFactory {
public:
	virtual ~IStateFactory() = default;

	virtual std::shared_ptr<IConnectionHandler> createSession(
			uint8_t qosLevel, 
			std::string const& password,
			std::string const& login,
			std::string const& secret,
			std::string const& topic) const = 0;

	virtual std::shared_ptr<IConnectionHandler> createSession(SessionConfig& sc) const = 0;
};
