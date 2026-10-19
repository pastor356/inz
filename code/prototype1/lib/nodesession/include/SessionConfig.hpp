#pragma once
#include <string>
#include <cstdint>
#include "IDataAccessor.hpp"

struct SessionConfig {
	uint8_t qosLevel; // 0, 1 or 2
	bool isProducer;
	bool isConsumer;
	bool creatFlag;
	bool newUsr;
	UserAuth authData;
	std::string topic;
	uint8_t rights; 
};

