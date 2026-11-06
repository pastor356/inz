#include "ConsoleLogger.hpp"

std::string ConsoleLogger::applicationName = "Default";
bool ConsoleLogger::enable = false;

ConsoleLogger::ConsoleLogger(ILogger::Severity sev, std::string const& msg) {
	if(enable) {
		std::string outputString;

		switch(sev) {
			case ILogger::Severity::LOG_INFO:
				outputString += "INFO: ";
				break;
			case ILogger::Severity::LOG_WARN:
				outputString += "WARNING: ";
				break;
			case ILogger::Severity::LOG_ERROR:
				outputString += "ERROR: ";
				break;
			default:
				outputString += "Unknown logtype: ";
				break;
		}

		outputString += msg;

		std::cout << outputString << std::endl;
	}
}


ConsoleLogger::ConsoleLogger(ILogger::Severity sev, std::string file, int line, std::string func) {
	if(enable) {
		std::string outputString;

		switch(sev) {
			case ILogger::Severity::LOG_INFO:
				outputString += "INFO:";
				break;
			case ILogger::Severity::LOG_WARN:
				outputString += "WARNING:";
				break;
			case ILogger::Severity::LOG_ERROR:
				outputString += "ERROR:";
				break;
			default:
				outputString += "Unknown logtype: ";
				break;
		}

		outputString += file;
		outputString += "::" + std::to_string(line);
		std::cout << applicationName << "::" << outputString 
			<< " in " << std::endl << '\t' << func << "(): \t";
	}
}


ILogger& ConsoleLogger::operator << (std::string const& msg) { 
	if(enable) std::cout << msg << std::endl << std::endl;

	return(*this);
}

void ConsoleLogger::setApplicationName(std::string const& appName) {
	applicationName = appName;
} 

bool ConsoleLogger::enableLogger() {
	ConsoleLogger::enable = true;
	return(enable);
}

