//Understanding is a three edged sword.

#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>
#include "ConfigLoader.h"
#include "../View/Render.h"


void ConfigLoader::bind(Render* render) {
    this->render = render;
}

void ConfigLoader::readCSV(std::string fileName) {
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

void ConfigLoader::loadConfig() {
    readCSV(configFile);
    for (size_t i = 1; i < parsedCSV.size(); i++) {
        std::string conifName = parsedCSV[i][0];

        if (conifName == "displayMode") {
            std::string displayMode = parsedCSV[i][1];

            if (displayMode == "windowed") {
                render->setToWindowed();
            }

            if (displayMode == "borderless") {
                render->setToBorderless();
            }

            if (displayMode == "fullscreen") {
                render->setToFullScreen();
            }
        }
    }
}