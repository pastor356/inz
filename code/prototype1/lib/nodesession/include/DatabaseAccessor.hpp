#pragma once
#include "IDataAccessor.hpp"
#include "SQLiteCpp/SQLiteCpp.h"
#include "SQLiteCpp/Database.h"
#include "SQLiteCpp/Statement.h"

/*
   CREATE TABLE usr_auth(
	   login TEXT NOT NULL, 
	   password TEXT NOT NULL, 
	   secret TEXT NOT NULL, 
	   rights INTEGER CHECK (rights > 0 AND rights < 4), 
	   sessionId INTEGER CHECK (sessionId > 0), 
	   topic TEXT, 
	   sessActive INTEGER);
*/


class DatabaseAccessor: public IDataAccessor {
	mutable SQLite::Database dbase;
	mutable SQLite::Statement query; // ?

	enum class DataPos: size_t {
		LOGIN = 0,
		PASSWORD = 1,
		SECRET = 2,
		RIGHTS = 3,
		SESSIONID = 4,
		TOPIC = 5
	};

public:
	DatabaseAccessor(std::string const& dbFilename);

	IDataAccessor::AccessorStatus authorize(UserAuth const& auth) const override;
	IDataAccessor::AccessorStatus createUser(UserAuth const& auth) const override;

	void getTopics(TopicSet& topicSet) const override;
	void addTopic(std::string const& topic) override;

	~DatabaseAccessor();
};



