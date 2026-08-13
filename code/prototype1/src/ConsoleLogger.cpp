#include "ConsoleLogger.hpp"

ConsoleLogger::ConsoleLogger(ILogger::Severity sev, std::string const& msg) {
	std::string outputString;

	switch(sev) {
		case ILogger::Severity::LOG_INFO:
			outputString += "INFO: ";
		case ILogger::Severity::LOG_WARN:
			outputString += "WARNING: ";
		case ILogger::Severity::LOG_ERROR:
			outputString += "ERROR: ";
		default:
			outputString += "Unknown logtype: ";
	}

	outputString += msg;

	std::cout << outputString << std::endl;
}


ConsoleLogger::ConsoleLogger(ILogger::Severity sev, std::string file, int line) {
	std::string outputString;

	outputString += file;
	outputString += line;

	std::cout << applicationName << outputString << ": ";
}


ILogger& ConsoleLogger::operator << (std::string const& msg) { 
	std::cout << msg << std::endl;
	return(*this);
}

void ConsoleLogger::setApplicationName(std::string const& appName) {
	applicationName = appName;
} 

