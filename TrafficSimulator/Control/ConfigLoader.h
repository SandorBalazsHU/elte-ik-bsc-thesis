/**
 * @name Traffic Simulation
 * @file ConfigLoader.h
 * @class ConfigLoader
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief The configuration loader class.
 * Contact: sandorbalazs9402@gmail.com
*/

//Understanding is a three edged sword.

#pragma once

//Avoiding the circular include.
class Render;

#include <string>
#include <vector>

/**
 * @brief The configuration loader class.
*/
class ConfigLoader {
public:
	/**
	 * @brief Bind the configuration logger for the render.
	 * @param render The current render pointer.
	*/
	void bind(Render* render);

	/**
	 * @brief Load and apply the configurations.
	*/
	void loadConfig();

private:

	/**
	 * @brief The config file location.
	*/
	std::string configFile = "config.csv";

	/**
	 * @brief The render pointer.
	*/
	Render* render = NULL;

	/**
	 * @brief The parsed CSV configuration file in the memory.
	*/
	std::vector<std::vector<std::string>> parsedCSV;

	/**
	 * @brief Read and parse the configuration CSV file.
	 * @param fileName The configuration file name.
	*/
	void readCSV(std::string fileName);
};