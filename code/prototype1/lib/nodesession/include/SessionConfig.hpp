#pragma once
#include <string>
#include <cstdint>

struct SessionConfig {
	uint8_t qosLevel; // 0, 1 or 2
	bool isProducer;
	bool creatFlag;
	bool newUsr;
	std::string login;
	std::string passwd;
	std::string secret;
	std::string topic;
	uint8_t rights; 
};

