#pragma once 
#include "ILogger.hpp"

class ConsoleLogger: public ILogger {

protected:
	static std::string applicationName;
	static bool enable;
	static bool initialized;

public:

	ILogger& operator << (std::string const& msg) override;
	static bool enableLog();

	static void setApplicationName(std::string const& appName); 

	ConsoleLogger(ILogger::Severity sev, std::string const& msg);
	ConsoleLogger(ILogger::Severity sev, std::string file, int line);
};


#define logError ConsoleLogger(ILogger::Severity::LOG_ERROR, __FILE__, __LINE__)
#define logInfo ConsoleLogger(ILogger::Severity::LOG_INFO, __FILE__, __LINE__)
#define logWarn ConsoleLogger(ILogger::Severity::LOG_WARN, __FILE__, __LINE__)

#define enableLogger ConsoleLogger::enableLog()

#define configureLogger(name) ConsoleLogger::setAplicationName(name)
