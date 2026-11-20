#include "ActiveQueue.hpp"


void ActiveQueue::insert(std::shared_ptr<MethodRequest> request) {
	mtx.lock();
	activationList.push_back(request);
	blockEmpty.notify_one();
	mtx.unlock();
}


void ActiveQueue::remove(std::shared_ptr<MethodRequest> request) {
	mtx.lock();
	activationList.remove(request);

	if(!activationList.empty()) mtx.unlock();
}


std::shared_ptr<MethodRequest> ActiveQueue::begin() {
	std::list< std::shared_ptr<MethodRequest> >::iterator it;
	std::unique_lock<std::mutex> ulock(mtx, std::defer_lock);

	while(1) {
		if(!activationList.empty()) {
			std::lock_guard<std::mutex> lock(mtx);
			it = activationList.begin();
			break;
		} else { 
			ulock.lock();
			blockEmpty.wait(ulock);
		}
	}

	return(*it);
}


std::shared_ptr<MethodRequest> ActiveQueue::end() {
	std::unique_lock<std::mutex> lock(mtx);
	std::list<
		std::shared_ptr<MethodRequest>>::iterator it = activationList.end();

	return(*it);
}


bool ActiveQueue::empty() const {
	return(activationList.empty());
}
