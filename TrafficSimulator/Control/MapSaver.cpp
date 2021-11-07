#include <iostream>
#include <fstream>

#include "MapSaver.h"
#include "../View/Render.h"
#include "../View/Objects/Object3D.h"

MapSaver::MapSaver(void) {

}

MapSaver::~MapSaver(void) {

}

void MapSaver::bind(Render* render) {
	this->render = render;
}

void MapSaver::saveMap(std::string fileName) {
	std::ofstream saveFile;
	saveFile.open(this->saveFolder + fileName);

	for (size_t i = 0; i < render->renderableObjects.size(); i++) {
		int id = render->renderableObjects[i].getId();
		if ( id != 1 && id != 2 && id != 4) {
			saveFile << render->renderableObjects[i].getId() << ";";

			glm::vec3 position = render->renderableObjects[i].getPosition();
			saveFile << position.x << ";" << position.y << ";" << position.z << ";";

			glm::vec3 rotation = render->renderableObjects[i].getRotation();
			saveFile << rotation.x << std::endl;
		}
	}

	for (size_t i = 0; i < render->renderableRoads.size(); i++) {
		saveFile << "-1;";
		Object3D** trackBalls = render->renderableRoads[i]->getTrackBalls();
		for (size_t j = 0; j < 4; j++) {
			glm::vec3 position = trackBalls[j]->getPosition();
			saveFile << position.x << ";" << position.y << ";" << position.z << ";";
		}
		saveFile << ";" << std::endl;
	}

	saveFile.close();
}