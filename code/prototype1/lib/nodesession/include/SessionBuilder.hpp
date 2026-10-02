#pragma once
#include "DecoratorBuilder.hpp"
#include "Session.hpp"
#include "SessionConfig.hpp"

class SessionBuilder {
	std::shared_ptr<Session> result;
	std::unique_ptr<DecoratorBuilder> connectionBuilder;

public:
	SessionBuilder(std::unique_ptr<DecoratorBuilder> builder);
	void reset();  // this->result = std::make_shared
	std::shared_ptr<Session> getSession() const; // return(std::make_shared(result));

	void setConnection(std::unique_ptr<IConnection> conn);
	void setContext(std::shared_ptr<IStateContext> ctx);
	void setSessionConfig(std::string const& login, 
			std::string const& password, std::string const& secret);

	void setConfig(SessionConfig const& conf);
};
