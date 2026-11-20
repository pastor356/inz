#pragma once
#include "DatabaseAccessor.hpp"
#include "MethodRequest.hpp"
#include <future>


class AppendRequest: public MethodRequest {
	std::weak_ptr<DatabaseAccessor> dbAcc;
	Sample sampleToAppend;

	std::promise<DBStatus> fut;

public:
	AppendRequest(std::shared_ptr<DatabaseAccessor> db, Sample sample,
			std::promise<DBStatus> prom);

	std::future<DBStatus> getFuture();
	bool canRun() const override { return(true); }
	void run() override;
};
