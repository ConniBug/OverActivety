#pragma once
#include <string>
#include "structs.h"

namespace clog {
	std::string logLevelToStr(loggingLevel level) {
		switch (level) {
		case(loggingLevel::DEFAULT_t):
		case(loggingLevel::ERROR_t):
			return std::string("Error");
		case(loggingLevel::WARNING_t):
			return std::string("Warning");
		case(loggingLevel::DEBUGGING_t):
			return std::string("Debugging");
		}
		throw "logLevelToStrErr";
		return "";
	}

	void log(std::string message, std::string caller = "N/A", loggingLevel level = loggingLevel::DEFAULT_t) {
		switch (level) {
		case(loggingLevel::DEFAULT_t):
		case(loggingLevel::ERROR_t):
		case(loggingLevel::WARNING_t):
		case(loggingLevel::DEBUGGING_t):
			std::cout << "[TIME] [" << caller << "] (" << logLevelToStr(level) << ") - " + message << std::endl;
		}
	}
}