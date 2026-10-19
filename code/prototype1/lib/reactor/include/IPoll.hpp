#pragma once
#include "EventHandler.hpp"
#include "IStateContext.hpp"

class IPoll {
public:
	virtual void registerNode(std::shared_ptr<IStateContext> node, 
			std::shared_ptr<EventHandler> handler) = 0;

	virtual void deregister(std::shared_ptr<IStateContext> node) = 0;
	virtual void deregister(std::shared_ptr<EventHandler> handler) = 0; 

	virtual size_t wait() const = 0;
};
