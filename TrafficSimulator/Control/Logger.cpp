#define _CRT_SECURE_NO_WARNINGS

#include <ctime>
#include <string>
#include "Logger.h"

void Logger::start() {
	freopen("simulation.log", "w", stdout);
	freopen("error.log", "w", stderr);
}

std::string Logger::currentDateTime() {
	time_t now = time(0);
    tm* local_time = localtime(&now);
	std::string dateTime = "" + 
	std::to_string(local_time->tm_year) + ". " +
	std::to_string(local_time->tm_mon) + ". " +
	std::to_string(local_time->tm_mday) + ". " +
	std::to_string(local_time->tm_hour) + ":" +
	std::to_string(local_time->tm_min) + ":" +
	std::to_string(local_time->tm_sec) + ":";
	return dateTime;
}