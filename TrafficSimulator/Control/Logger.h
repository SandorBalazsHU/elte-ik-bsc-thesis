#pragma once
#include <iostream>

class Logger {
public:
	static void startFileLogging();
	static void stopFileLogging();
	static std::string currentDateTime();
	static void log(std::string message);
	static void error(std::string message);
	static void error(std::string message, int errorCode);

private:
	Logger() {}
};