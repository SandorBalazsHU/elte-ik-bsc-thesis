#include <iostream>
#include <fstream>

#include "MapSaver.h"
#include "../View/Render.h"
#include "../View/Objects/Object3D.h"
#include "Logger.h"

MapSaver::MapSaver(void) {

}

MapSaver::~MapSaver(void) {

}

void MapSaver::bind(Render* render) {
	this->render = render;
}

void MapSaver::reset() {
	currentSave = unsavedMarker;
	lastSaveTime = unsavedMarker;
}

std::string MapSaver::getLastSave() {
	return currentSave;
}

void MapSaver::setLastSave(std::string loadedMap) {
	currentSave = loadedMap;
	setLastSaveTime();
}

std::string MapSaver::getLastSaveTime() {
	return lastSaveTime;
}

void MapSaver::setLastSaveTime() {
	lastSaveTime = Logger::currentDateTime();
}

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