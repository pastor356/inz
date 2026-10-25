#pragma once
#include "IPoll.hpp"

class PollMock: public IPoll {
public:
	MOCK_METHOD((void), registerNode, ((std::shared_ptr<IStateContext>), 
				(std::shared_ptr<EventHandler>)));
	MOCK_METHOD((void), deregisterNode, (std::shared_ptr<IStateContext>));
	MOCK_METHOD((void), deregisterHandler, (std::shared_ptr<EventHandler>));
	MOCK_METHOD((std::vector<IPoll::ReadyEvent>), wait, (), (const, override));
};



