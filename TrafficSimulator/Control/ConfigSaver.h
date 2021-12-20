/**
 * @name Traffic Simulation
 * @file ConfigSaver.h
 * @class ConfigSaver
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief Configuration Saver class
 * Contact: sandorbalazs9402@gmail.com
*/

//Alarm 1202, Alarm 1201

#pragma once


#include <string>
#include <vector>

/**
 * @brief Configuration saver class.
*/
class ConfigSaver {
public:

	/**
	 * @brief Set and save a configuration property.
	 * @param propertyName The settable property name.
	 * @param value The settable property value.
	*/
	void setProperty(std::string propertyName, std::string value);

private:

	/**
	 * @brief The configuration file path and name.
	*/
	std::string configFile = "config.csv";

	/**
	 * @brief The parsed CSV file in the memory.
	*/
	std::vector<std::vector<std::string>> parsedCSV;

	/**
	 * @brief Write out the configuration data.
	 * @param fileName The configuration file name.
	*/
	void writeCSV(std::string fileName);

	/**
	 * @brief Read in the configuration data.
	 * @param fileName The configuration file name.
	*/
	void readCSV(std::string fileName);
};