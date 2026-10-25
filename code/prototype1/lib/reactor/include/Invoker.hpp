#pragma once
#include "IInvoker.hpp"

class Invoker: public IInvoker {
public:
	Invoker() = default;
	void dispatch(std::vector<IPoll::ReadyEvent>& events) override;
};
