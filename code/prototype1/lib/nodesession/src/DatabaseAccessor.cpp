#include "DatabaseAccessor.hpp"

DatabaseAccessor::DatabaseAccessor(std::string const& dbFilename):
	dbase(dbFilename, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE),
	query(dbase, "SELECT login, password,"
					"secret, rights, sessionId, topic "
					"FROM usr_auth WHERE login == ?")
{}

IDataAccessor::AccessorStatus DatabaseAccessor::authorize(UserAuth const& auth) const { 
	std::vector<std::string> dbResponse;

	query.bind(1, auth.login);
	query.executeStep();

	for(size_t i = 0; i < 6; ++i) dbResponse.push_back(query.getColumn(i));

	if(auth.password == dbResponse[DataPos::PASSWORD]) {
		if(auth.secret == dbResponse[DataPos::SECRET]) {
			if(auth.rights == dbResponse[DataPos::RIGHTS]) {
				return(IDataAccessor::AccessorStatus::SUCCESS);
			} else {
				return(IDataAccessor::AccessorStatus::FAILURE);
			}
		} else {
			return(IDataAccessor::AccessorStatus::INVALID_SECRET);
		}
	} else {
		return(IDataAccessor::AccessorStatus::INVALID_PASSWORD);
	}
}


void Database::getTopics(TopicSet& topicSet) const {
	std::vector<std::string> dbResponse;


