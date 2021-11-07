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
            //int newRoadRenderId = render->addRoad();
        }
        std::cout << currentType << ". Object added" << std::endl;
    }
}