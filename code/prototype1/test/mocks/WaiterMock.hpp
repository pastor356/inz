#include "IWaiter.hpp"

class WaiterMock: public IWaiter {
public:
	MOCK_METHOD(int, create, (), (override));
	MOCK_METHOD(int, wait, (int, struct epoll_event*, int, int), (override));
	MOCK_METHOD(int, ctl, (int, int, int, struct epoll_event*), (override));
};

