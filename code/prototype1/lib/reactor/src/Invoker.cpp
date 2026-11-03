#include "Invoker.hpp"

void Invoker::dispatch(std::vector<IPoll::ReadyEvent>& events) {
	size_t vecSiz = events.size();

	int eventFlags;
	for(size_t i = 0; i < vecSiz; ++i) {

		if(events[i].entity && events[i].entity->handler && events[i].entity->node) {
			eventFlags = events[i].flags;

			if(eventFlags & EPOLLERR) 
				events[i].entity->handler->handleError(events[i].entity->node);

			if(eventFlags & EPOLLHUP | EPOLLRDHUP) 
				events[i].entity->handler->handleHup(events[i].entity->node);

			if(eventFlags & EPOLLIN) 
				events[i].entity->handler->handleRead(events[i].entity->node);

			if(eventFlags & EPOLLOUT) 
				events[i].entity->handler->handleWrite(events[i].entity->node);

		} else continue;
	}
}
