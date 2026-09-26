#pragma once
#include "IConnectionHandler.hpp"

class IStateFabric {
public:
	virtual ~IStateFabric() = default;

	virtual std::shared_ptr<IConnectionHandler> createSession(
			uint8_t qosLevel, 
			std::string const& password,
			std::string const& login,
			std::string const& secret,
			std::string const& topic) const = 0;

};




