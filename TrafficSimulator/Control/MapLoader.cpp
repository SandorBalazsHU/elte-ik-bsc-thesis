#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>
#include "MapLoader.h"
#include "../View/Render.h"

/*MapLoader::MapLoader(void) {

}

MapLoader::~MapLoader(void) {

}*/

void MapLoader::bind(Render* render) {
	this->render = render;
}

void MapLoader::readCSV(std::string fileName) {
    parsedCSV.clear();
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
    readCSV(this->saveFolder + fileName + fileType);
    for (size_t i = 0; i < parsedCSV.size(); i++) {
        int currentType = std::stoi(parsedCSV[i][0]);

        if (currentType != -1) {
            int newObjecRenderId = render->addObject(currentType);
            render->getObject(newObjecRenderId)->setPosition(glm::vec3(std::stof(parsedCSV[i][1]), std::stof(parsedCSV[i][2]), std::stof(parsedCSV[i][3])));
            render->getObject(newObjecRenderId)->setRotation(glm::vec4(std::stof(parsedCSV[i][4]), 0, 1, 0));
        } else {
            int newRoadRenderId = render->addRoad();
            size_t* trackBalls = render->getDynamicObject(newRoadRenderId)->getTrackBalls();
            render->getObject(trackBalls[0])->setPosition(glm::vec3(std::stof(parsedCSV[i][1]), std::stof(parsedCSV[i][2]), std::stof(parsedCSV[i][3])));
            render->getObject(trackBalls[1])->setPosition(glm::vec3(std::stof(parsedCSV[i][4]), std::stof(parsedCSV[i][5]), std::stof(parsedCSV[i][6])));
            render->getObject(trackBalls[2])->setPosition(glm::vec3(std::stof(parsedCSV[i][7]), std::stof(parsedCSV[i][8]), std::stof(parsedCSV[i][9])));
            render->getObject(trackBalls[3])->setPosition(glm::vec3(std::stof(parsedCSV[i][10]), std::stof(parsedCSV[i][11]), std::stof(parsedCSV[i][12])));
            render->updateDynamicObject(newRoadRenderId);
        }
    }
    for (size_t i = 0; i < render->getDynamicObjectsNumber(); i++) {
        if (render->getDynamicObject(i) != NULL) {
            render->updateDynamicObject(i);
        }
    }
}

std::vector<std::string> MapLoader::listFiles() {
    std::vector<std::string> fileList;
    for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(saveFolder)) {
        std::string fileName = entry.path().filename().string();
        fileList.push_back(fileName.substr(0, fileName.find(".")));
    }
    return fileList;
}

int MapLoader::deleteSave(std::string fileName) {
    try {
        if (std::filesystem::remove(saveFolder + fileName + fileType))
            return 0;
        else
            return 1;
    }
    catch (const std::filesystem::filesystem_error& err) {
        std::cout << "filesystem error: " << err.what() << '\n';
        return 2;
    }
}