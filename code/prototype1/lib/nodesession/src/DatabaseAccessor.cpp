#include "DatabaseAccessor.hpp"

SQLite::Database DatabaseAccessor::openAndInit(const std::string& path) {
    SQLite::Database db{path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE};
    db.exec("PRAGMA foreign_keys = ON");
    db.exec(R"sql(
        CREATE TABLE IF NOT EXISTS topics (
            topic TEXT NOT NULL PRIMARY KEY
        );
        CREATE TABLE IF NOT EXISTS usr (
            login    TEXT    NOT NULL PRIMARY KEY,
            password TEXT    NOT NULL,
            secret   TEXT    NOT NULL,
            rights   INTEGER NOT NULL CHECK (rights BETWEEN 1 AND 3),
            topic    TEXT    REFERENCES topics(topic) ON DELETE SET NULL
        );
    )sql");
    return db;
}


DatabaseAccessor::DatabaseAccessor(std::string const& dbFilename): dbase(openAndInit(dbFilename)),
      authorizeQuery(dbase,
          "SELECT password, secret, rights, topic "
          "FROM usr WHERE login = ?"),                                 
      createUserQuery(dbase,
          "INSERT INTO usr(login, password, secret, rights) "
          "VALUES (?, ?, ?, ?)"),
      getTopicsQuery(dbase,
          "SELECT topic FROM topics"),
      addTopicQuery(dbase,
          "INSERT OR IGNORE INTO topics(topic) VALUES (?)")
{} 


IDataAccessor::AccessorStatus DatabaseAccessor::authorize(UserAuth const& auth) const { 
	std::vector<std::string> dbResponse;

	authorizeQuery.bind(1, auth.login);
	authorizeQuery.executeStep();

	ssize_t ncols;
	try {
		ncols = authorizeQuery.getColumnCount();
	} catch(std::exception &ex) {
		logError << ex.what();
		return(IDataAccessor::AccessorStatus::ERROR);
	}

	try {
		for(size_t i = 0; i < ncols; ++i) 
			dbResponse.push_back(query.getColumn(i));
	} catch(std::exception& ex) {
		logError << ex.what();
	}

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

	getTopicsQuery.executeStep();

	size_t ncols = getTopicsQuery.getColumnCount(); 

	try {
		for(size_t i = 0; i < ncols; ++i) 
			dbResponse.push_back(query.getColumn(i));
	} catch(std::exception& ex) {
		logError << ex.what();
		return(IDataAccessor::AccessorStatus::ERROR);
	}

	for(auto topic: dbResponse) {
		topicSet.insert(topic);
	}

	return(IDataAccessor::AccessorStatus::SUCCESS);
}


IDataAccessor::AccessorStatus DatabaseAccessor::createUser(UserAuth const& auth) const {
	std::vector<std::string> dbResponse;
	ssize_t ncols;

	createUserQuery.bind(1, auth.login);
	createUserQuery.bind(2, auth.password);
	createUserQuery.bind(3, auth.secret);
	createUserQuery.bind(4, auth.rights);
	createUserQuery.bind(5, auth.rights);
	createUserQuery.executeStep();

	ncols = createUserQuery.getColumnCount();

	try {
		for(size_t i = 0; i < ncols; ++i) 
			dbResponse.push_back(query.getColumn(i));
	} catch(std::exception& ex) {
		logError << ex.what();
		return(IDataAccessor::AccessorStatus::ERROR);
	}

	return(IDataAccessor::AccessorStatus::SUCCESS);
}
