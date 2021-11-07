#include <sstream>
#include <fstream>
#include "MapLoader.h"
#include "../View/Render.h"

MapLoader::MapLoader(void) {

}

MapLoader::~MapLoader(void) {

}

void MapLoader::bind(Render* render) {
	this->render = render;
}

void MapLoader::readCSV(std::string fileName) {
    std::ifstream file(fileName);
    std::string line;
    int lineCount = 0;
    while (std::getline(file, line)) {
        std::stringstream lineStream(line);
        std::string cell;
        std::vector<std::string> parsedRow;
        while (std::getline(lineStream, cell, ';')) {
            parsedRow.push_back(cell);
        }
        parsedCSV.push_back(parsedRow);
        lineCount++;
    }
    file.close();
}

void MapLoader::loadMap(std::string fileName) {
    readCSV(this->saveFolder + fileName);
    for (size_t i = 0; i < parsedCSV.size(); i++) {
        int currentType = std::stoi(parsedCSV[i][0]);

        if (currentType != -1) {
            int newObjecRenderId = render->addObject(currentType);
            render->getObject(newObjecRenderId)->setPosition(glm::vec3(std::stof(parsedCSV[i][1]), std::stof(parsedCSV[i][2]), std::stof(parsedCSV[i][3])));
            render->getObject(newObjecRenderId)->setRotation(glm::vec4(std::stof(parsedCSV[i][4]), 0, 1, 0));
        } else {
            int newRoadRenderId = render->addRoad();
            Object3D** trackBalls = render->renderableRoads[newRoadRenderId]->getTrackBalls();
            trackBalls[0]->setPosition(glm::vec3(std::stof(parsedCSV[i][1]), std::stof(parsedCSV[i][2]), std::stof(parsedCSV[i][3])));
            trackBalls[1]->setPosition(glm::vec3(std::stof(parsedCSV[i][4]), std::stof(parsedCSV[i][5]), std::stof(parsedCSV[i][6])));
            trackBalls[2]->setPosition(glm::vec3(std::stof(parsedCSV[i][7]), std::stof(parsedCSV[i][8]), std::stof(parsedCSV[i][9])));
            trackBalls[3]->setPosition(glm::vec3(std::stof(parsedCSV[i][10]), std::stof(parsedCSV[i][11]), std::stof(parsedCSV[i][12])));
            render->updateDynamicObject(newRoadRenderId);
        }
        std::cout << currentType << ". Object added" << std::endl;
    }
}