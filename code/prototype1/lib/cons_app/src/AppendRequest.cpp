#include "AppendRequest.hpp"

AppendRequest::AppendRequest(std::shared_ptr<DatabaseAccessor> db, Sample sample, 
		std::promise<DBStatus> prom): 
	dbAcc{db}, sampleToAppend{sample}, fut{std::move(prom)} {}

void AppendRequest::run() {

	if(auto strong = dbAcc.lock()) {

		try{
			fut.set_value(strong->append(sampleToAppend));
		} catch(...) {
			fut.set_exception(std::current_exception());
		}

	}
}


std::future<DBStatus> AppendRequest::getFuture() {
	return(fut.get_future());
}
