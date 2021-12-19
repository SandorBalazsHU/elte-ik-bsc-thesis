#define _CRT_SECURE_NO_WARNINGS

#include <ctime>
#include <string>
#include "Logger.h"

void Logger::startFileLogging() {
	const bool logToFile = true;
	if (logToFile) {
		freopen("Log/simulation.log", "w", stdout);
		freopen("Log/error.log", "w", stderr);
	}
}

void Logger::stopFileLogging() {
	fclose(stdout);
	fclose(stderr);
}

std::string Logger::currentDateTime() {
	time_t now = time(0);
    tm* local_time = localtime(&now);
	std::string dateTime = "" + 
	std::to_string(1900 + local_time->tm_year) + "." +
	std::to_string(local_time->tm_mon) + "." +
	std::to_string(local_time->tm_mday) + ". " +
	std::to_string(local_time->tm_hour) + ":" +
	std::to_string(local_time->tm_min) + ":" +
	std::to_string(local_time->tm_sec);
	return dateTime;
}

void Logger::log(std::string message) {
	std::cout << currentDateTime() << " - " << message << std::endl;
}

void Logger::error(std::string message) {
	std::cerr << currentDateTime() << " - " << message << std::endl;
}

void Logger::error(std::string message, int errorCode) {
	std::cerr << currentDateTime() << " - " << message << " ERROR CODE:" <<  errorCode << std::endl;
}