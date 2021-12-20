/**
 * @name Traffic Simulation
 * @file Logger.h
 * @class Logger
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief Logging system implementation. (Static class.)
 * Contact: sandorbalazs9402@gmail.com
*/

#pragma once

#include <iostream>

/**
 * @brief Logging system implementation. (Static class.)
*/
class Logger {
public:

	/**
	 * @brief Start and initialize the file logging and open the log files.
	*/
	static void startFileLogging();

	/**
	 * @brief Stop the file logging and close the log files.
	*/
	static void stopFileLogging();

	/**
	 * @brief Get the current date and time in String.
	 * @return The current date and time.
	*/
	static std::string currentDateTime();

	/**
	 * @brief Log a message.
	 * @param message The log message.
	*/
	static void log(std::string message);

	/**
	 * @brief Log an error.
	 * @param message The error message.
	*/
	static void error(std::string message);

	/**
	 * @brief Log an error and error code.
	 * @param message The error message.
	 * @param errorCode The error code.
	*/
	static void error(std::string message, int errorCode);

private:

	/**
	 * @brief The constructor is private and empty because is a static class.
	*/
	Logger() {}
};