#include "object3DLoader.h"
//#include <iostream>
#include <sstream>
#include <fstream>

object3Dloader::object3Dloader(void) {
}

object3Dloader::~object3Dloader(void) {
}

void object3Dloader::load() {
    readCSV();
    for (size_t i = 0; i < parsedCSV.size(); i++) {
        std::string currentType = parsedCSV[i][1];
        if (currentType == "texture") {
            std::string fileName = parsedCSV[i][2]; 
            textures[fileName] = Texture2D();
            textures[fileName].FromFile(textureFolder + fileName);
        }
        if (currentType == "vehicle") {
            for (size_t j = 3; j <= 7; j++) {
                std::string fileName = parsedCSV[i][j];
                textures[fileName] = Texture2D();
                textures[fileName].FromFile(textureFolder + fileName);
            }
        }
    }
}

void object3Dloader::readCSV() {
    std::ifstream file(configFile);
    std::string line;
    bool firstLineSkip = true;
    while (std::getline(file, line)) {
        if (!firstLineSkip) {
            std::stringstream lineStream(line);
            std::string cell;
            std::vector<std::string> parsedRow;
            while (std::getline(lineStream, cell, ';')) {
                parsedRow.push_back(cell);
                //std::cout << cell << " ";
            }
            parsedCSV.push_back(parsedRow);
            //std::cout << std::endl;
        } else {
            firstLineSkip = false;
        }
    }
	file.close();
}
