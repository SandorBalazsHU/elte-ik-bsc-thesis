#include "objectStorage.h"
//#include <iostream>
#include <sstream>
#include <fstream>

objectStorage::objectStorage(void) {
}

objectStorage::~objectStorage(void) {
}

void objectStorage::load() {
    readCSV();
    std::vector<std::thread> threads;

    for (size_t i = 0; i < parsedCSV.size(); i++) {

        std::string currentType = parsedCSV[i][1];

        if (currentType == "icon") {
            std::string fileName = parsedCSV[i][2];
            windowIcon = IMG_Load((textureFolder + fileName).c_str());
        }

        if (currentType == "texture") {
            std::string fileName = parsedCSV[i][2];
            if (!isThisTextureLoaded(fileName))
            threads.push_back(loadTextureParallel(fileName));
        }
        if (currentType == "vehicle") {
            for (size_t j = 3; j <= 7; j++) {
                std::string fileName = parsedCSV[i][j];
                if(!isThisTextureLoaded(fileName))
                threads.push_back(loadTextureParallel(fileName));
            }
        }
    }

    for (size_t i = 0; i < threads.size(); i++) {
        threads[i].join();
    }

    std::map<std::string, Texture2D>::iterator it;
    for (it = textures.begin(); it != textures.end(); it++) {
        it->second.bindTexture();
    }
}

void objectStorage::loadTexture(std::string fileName) {
    texturesMutex.lock();
    Texture2D& texture = textures[fileName];
    texturesMutex.unlock();
    texture.FromFileParallel(textureFolder + fileName);
}

std::thread objectStorage::loadTextureParallel(std::string fileName) {
    return std::thread([=]{loadTexture(fileName);});
}

void objectStorage::readCSV() {
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
            }
            parsedCSV.push_back(parsedRow);
        } else {
            firstLineSkip = false;
        }
    }
	file.close();
}

bool objectStorage::isThisTextureLoaded(std::string textureName) {
    return(textures.find(textureName) != textures.end());
}