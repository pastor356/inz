#pragma once
#include "IPoll.hpp"

class IInvoker {
public:
	virtual void dispatch(std::vector<IPoll::ReadyEvent>& events) = 0;
	virtual ~IInvoker() = default;
};
