#pragma once
#include <vector>
#include "IDataAccessor.hpp"
#include "SQLiteCpp/SQLiteCpp.h"
#include "SQLiteCpp/Database.h"
#include "SQLiteCpp/Statement.h"

/*

sqlite> .schema topics
CREATE TABLE topics (
	topic TEXT NOT NULL PRIMARY KEY
);

CREATE TABLE usr(
	login TEXT NOT NULL PRIMARY KEY,
	password TEXT NOT NULL,
	secret TEXT NOT NULL,
	topic TEXT REFERENCES topics(topic)
);
*/

class DatabaseAccessor: public IDataAccessor {
	mutable SQLite::Database dbase;

	mutable SQLite::Statement authorizeQuery; 
	mutable SQLite::Statement createUserQuery;
	mutable SQLite::Statement getTopicsQuery;
	mutable SQLite::Statement addTopicQuery;

	enum class DataPos: size_t {
		LOGIN = 0,
		PASSWORD = 1,
		SECRET = 2,
		RIGHTS = 3,
		SESSIONID = 4,
		TOPIC = 5
	};

	SQLite::Database openAndInit(const std::string& path);

public:
	DatabaseAccessor(std::string const& dbFilename);

	IDataAccessor::AccessorStatus authorize(UserAuth const& auth) const override;
	IDataAccessor::AccessorStatus createUser(UserAuth const& auth) const override;

	void getTopics(TopicSet& topicSet) const override;
	void addTopic(std::string const& topic) override;

	~DatabaseAccessor();
};
