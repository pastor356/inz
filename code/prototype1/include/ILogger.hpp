#pragma once 
#include <string>
#include <cstdint>
#include <iostream>


class ILogger {

public:
	static void setAplicationName(std::string const& appName);
	virtual ILogger& operator << (std::string const& msg) = 0;

	enum class Severity: uint16_t {
		LOG_INFO,
		LOG_WARN,
		LOG_ERROR
	};
};

