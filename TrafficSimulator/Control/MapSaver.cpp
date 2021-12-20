/**
 * @name Traffic Simulation
 * @file MapSaver.h
 * @class MapSaver
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief Map saver mechanism.
 * Contact: sandorbalazs9402@gmail.com
*/

#include <iostream>
#include <fstream>
#include "MapSaver.h"
#include "Logger.h"
#include "../View/Render.h"
#include "../View/Objects/Object3D.h"

/**
 * @brief Map saver constructor.
 * @param  Empty.
*/
MapSaver::MapSaver(void) {

}

/**
 * @brief Map saver destructor.
 * @param  Empty.
*/
MapSaver::~MapSaver(void) {

}

/**
 * @brief Bind the map saver to the render.
 * @param render Pointer to the current render.
*/
void MapSaver::bind(Render* render) {
	this->render = render;
}

/**
 * @brief Reset.
*/
void MapSaver::reset() {
	currentSave = unsavedMarker;
	lastSaveTime = unsavedMarker;
}

/**
 * @brief Get the last save name.
 * @return The last save name.
*/
std::string MapSaver::getLastSave() {
	return currentSave;
}

/**
 * @brief Set the last save name.
 * @param The last save name.
*/
void MapSaver::setLastSave(std::string loadedMap) {
	currentSave = loadedMap;
	setLastSaveTime();
}

/**
 * @brief Get the last save time.
 * @return The last save time.
*/
std::string MapSaver::getLastSaveTime() {
	return lastSaveTime;
}

/**
 * @brief Set the last save time.
 * @param The last save time.
*/
void MapSaver::setLastSaveTime() {
	lastSaveTime = Logger::currentDateTime();
}

/**
 * @brief Save the map to the file.
 * @param fileName The save file name.
*/
void MapSaver::saveMap(std::string fileName) {
	currentSave = fileName;
	setLastSaveTime();
	std::ofstream saveFile;
	saveFile.open(this->saveFolder + fileName + fileType);

	for (size_t i = 0; i < render->getObjectsNumber(); i++) {
		if (!render->getObject(i)->isDeleted()) {
			int id = render->getObject(i)->getId();
			if (id != 1 && id != 2 && id != 4) {
				saveFile << render->getObject(i)->getId() << ";";

				glm::vec3 position = render->getObject(i)->getPosition();
				saveFile << position.x << ";" << position.y << ";" << position.z << ";";

				glm::vec3 rotation = render->getObject(i)->getRotation();
				saveFile << rotation.x << std::endl;
			}
		}
	}

	for (size_t i = 0; i < render->getDynamicObjectsNumber(); i++) {
		if (render->getDynamicObject(i) != NULL) {
			saveFile << "-1;";
			size_t* trackBalls = render->getDynamicObject(i)->getTrackBalls();
			for (size_t j = 0; j < 4; j++) {
				glm::vec3 position = render->getObject(trackBalls[j])->getPosition();
				if (j < 3) {
					saveFile << position.x << ";" << position.y << ";" << position.z << ";";
				} else {
					saveFile << position.x << ";" << position.y << ";" << position.z;
				}
			}
			saveFile << std::endl;
		}
	}

	saveFile.close();
}