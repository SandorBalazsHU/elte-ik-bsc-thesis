/**
 * @name Traffic Simulation
 * @file MapLoader.h
 * @class MapLoader
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief Map loader implementation.
 * Contact: sandorbalazs9402@gmail.com
*/

#pragma once

/**
 * @brief Circular invlude prevention.
*/
class Render;

#include <string>
#include <vector>

/**
 * @brief Map loader class.
*/
class MapLoader {
public:

	/**
	 * @brief Bind map loader to the render.
	 * @param render Pointer for the current render.
	*/
	void bind(Render* render);

	/**
	 * @brief Load a map from CSV file.
	 * @param fileName The loadable map name.
	*/
	void loadMap(std::string fileName);

	/**
	 * @brief List of the exist maps in the saves folder.
	 * @return List of the exist maps.
	*/
	std::vector<std::string> listFiles();

	/**
	 * @brief Delete a saved map.
	 * @param fileName The deletable map.
	 * @return The success code of the deleting.
	*/
	int deleteSave(std::string fileName);

private:

	/**
	 * @brief The file type of the save files.
	*/
	const std::string fileType = ".csv";

	/**
	 * @brief The save folder.
	*/
	std::string saveFolder = "Saves/";

	/**
	 * @brief The render pointer.
	*/
	Render* render = NULL;

	/**
	 * @brief The parsed CSV save.
	*/
	std::vector<std::vector<std::string>> parsedCSV;

	/**
	 * @brief Read the CSV file to the memory.
	 * @param fileName The readable file name.
	*/
	void readCSV(std::string fileName);
};