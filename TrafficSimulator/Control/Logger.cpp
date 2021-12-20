/**
 * @name Traffic Simulation
 * @file Logger.cpp
 * @class Logger
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief Logging system implementation. (Static class.)
 * Contact: sandorbalazs9402@gmail.com
*/

#define _CRT_SECURE_NO_WARNINGS

#include <ctime>
#include <string>
#include "Logger.h"

/**
 * @brief Start and initialize the file logging and open the log files.
*/
void Logger::startFileLogging() {
	const bool logToFile = true;
	if (logToFile) {
		freopen("Log/simulation.log", "w", stdout);
		freopen("Log/error.log", "w", stderr);
	}
}

/**
 * @brief Stop the file logging and close the log files.
*/
void Logger::stopFileLogging() {
	fclose(stdout);
	fclose(stderr);
}

/**
 * @brief Get the current date and time in String.
 * @return The current date and time.
*/
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

/**
 * @brief Log a message.
 * @param message The log message.
*/
void Logger::log(std::string message) {
	std::cout << currentDateTime() << " - " << message << std::endl;
}

/**
 * @brief Log an error.
 * @param message The error message.
*/
void Logger::error(std::string message) {
	std::cerr << currentDateTime() << " - " << message << std::endl;
}

/**
 * @brief Log an error and error code.
 * @param message The error message.
 * @param errorCode The error code.
*/
void Logger::error(std::string message, int errorCode) {
	std::cerr << currentDateTime() << " - " << message << " ERROR CODE:" <<  errorCode << std::endl;
}