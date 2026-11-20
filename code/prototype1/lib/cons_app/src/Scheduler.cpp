#include "Scheduler.hpp"

void* thr_func(void* arg) {
	if(arg == nullptr) {
		return(nullptr);
	}

	Scheduler* thisObj = static_cast<Scheduler*>(arg);

	thisObj->dispatch();

	return(nullptr);
}


void* (*Scheduler::func)(void* arg) = thr_func;


Scheduler::Scheduler(std::unique_ptr<ActiveQueue> que): 
	activeQueue{std::move(que)} {

	thr = std::thread{func, this};
}

void Scheduler::insert(std::shared_ptr<MethodRequest> request) {
	activeQueue->insert(std::move(request));
}


void Scheduler::dispatch() {

	for(auto request = activeQueue->begin(); request != activeQueue->end();) {
		if(request->canRun()) {
			activeQueue->remove(std::move(request));
			request->run();
		}
	}

}

Scheduler::~Scheduler() {
	thr.join();
}
