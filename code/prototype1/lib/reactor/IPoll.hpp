#pragma once
#include "EventHandler.hpp"

class IPoll {
public:
	enum class EventType: uint8_t {
		READ,
		WRITE,
		HUP,
		ERROR
	};

	void register(std::shared_ptr<EventHandler> handler, 
			std::shared_ptr<IStateContext> ctx, EventType event);

	void deregister(std::shared_ptr<EventHandler> handler);
	uint8_t wait(

};
