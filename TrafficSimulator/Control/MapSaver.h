/**
 * @name Traffic Simulation
 * @file MapSaver.h
 * @class MapSaver
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief Map saver mechanism.
 * Contact: sandorbalazs9402@gmail.com
*/

#pragma once

/**
 * @brief For avoiding circular includes.
*/
class Render;

#include <string>

/**
 * @brief Map saver mechanism implementation.
*/
class MapSaver {
public:

	/**
	 * @brief Map saver constructor.
	 * @param  Empty.
	*/
	MapSaver(void);

	/**
	 * @brief Map saver destructor.
	 * @param  Empty.
	*/
	~MapSaver(void);

	/**
	 * @brief Bind the map saver to the render.
	 * @param render Pointer to the current render.
	*/
	void bind(Render* render);

	/**
	 * @brief Save the map to the file.
	 * @param fileName The save file name.
	*/
	void saveMap(std::string fileName);

	/**
	 * @brief Get the last save name.
	 * @return The last save name.
	*/
	std::string getLastSave();

	/**
	 * @brief Set the last save name.
	 * @param The last save name.
	*/
	void setLastSave(std::string loadedMap);

	/**
	 * @brief Get the last save time.
	 * @return The last save time.
	*/
	std::string getLastSaveTime();

	/**
	 * @brief Set the last save time.
	 * @param The last save time.
	*/
	void setLastSaveTime();

	/**
	 * @brief Reset.
	*/
	void reset();

	/**
	 * @brief The unsaved map name.
	*/
	const std::string unsavedMarker = "Unsaved";

private:

	/**
	 * @brief The file type.
	*/
	const std::string fileType = ".csv";

	/**
	 * @brief The current save name.
	*/
	std::string currentSave = unsavedMarker;

	/**
	 * @brief The save folder.
	*/
	const std::string saveFolder = "Saves\\";

	/**
	 * @brief The last save time in string.
	*/
	std::string lastSaveTime = unsavedMarker;

	/**
	 * @brief The current render pointer.
	*/
	Render* render;
};