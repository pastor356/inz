#include "SessionFactory.hpp"

std::shared_ptr<Session> createSession(
			uint8_t qosLevel, 
			std::string const& password,
			std::string const& login,
			std::string const& secret,
			std::string const& topic) const { 

	Session session;
	Session(


	return(std::make_shared<Session>{session});
}


