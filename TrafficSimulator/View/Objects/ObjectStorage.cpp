#include "ObjectStorage.h"

//#include <iostream>
#include <sstream>
#include <fstream>

ObjectStorage::ObjectStorage(void) {
}

ObjectStorage::~ObjectStorage(void) {
}

void ObjectStorage::load() {
    readCSV();
    for (size_t i = 0; i < parsedCSV.size(); i++) {
        std::string currentType = parsedCSV[i][2];

        if (currentType == "icon") {
            std::string fileName = parsedCSV[i][3];
            windowIcon = IMG_Load((textureFolder + fileName).c_str());
        }

        if (currentType == "texture") { 
            std::string fileName = parsedCSV[i][3];
            if (!isThisTextureLoaded(fileName) && fileName != "*") threads.push_back(loadTextureParallel(fileName));

            fileName = parsedCSV[i][4];
            if (!isThisTextureLoaded(fileName) && fileName != "*") threads.push_back(loadTextureParallel(miniatureFolder + fileName));
        }

        if ((currentType == "object") || (currentType == "desk") || (currentType == "mark")) {
            std::string fileName = parsedCSV[i][3];
            if (!isThisObjectLoaded(fileName)) threads.push_back(loadObjectParallel(fileName));

            fileName = parsedCSV[i][4];
            if (!isThisTextureLoaded(fileName) && fileName != "*") threads.push_back(loadTextureParallel(miniatureFolder+fileName));

            fileName = parsedCSV[i][5];
            if (!isThisTextureLoaded(fileName)) threads.push_back(loadTextureParallel(fileName));

            store(i);
            
            std::string isInTheFirstScene = parsedCSV[i][32];
            if (isInTheFirstScene == "1") firstSceneElements.push_back(std::stof(parsedCSV[i][0]));
        }

        if (currentType == "vehicle") {
            std::string fileName = parsedCSV[i][3];
            if (!isThisObjectLoaded(fileName)) threads.push_back(loadObjectParallel(fileName));

            fileName = parsedCSV[i][4];
            if (!isThisTextureLoaded(fileName)) threads.push_back(loadTextureParallel(miniatureFolder + fileName));

            for (size_t j = 5; j <= 9; j++) {
                std::string fileName = parsedCSV[i][j];
                if (!isThisTextureLoaded(fileName)) threads.push_back(loadTextureParallel(fileName));
            }

            store(i);
        }
    }
    loadingStateMax = threads.size();
}

void ObjectStorage::store(int csvID) {
    int id                       = std::stof(parsedCSV[csvID][0]);
    std::string name             = parsedCSV[csvID][1];
    std::string type             = parsedCSV[csvID][2];
    std::string meshID           = parsedCSV[csvID][3];
    std::string textureID        = parsedCSV[csvID][5];
    std::string iconID           = miniatureFolder + parsedCSV[csvID][4];
    std::string lightTexture     = parsedCSV[csvID][6];
    std::string breakTexture     = parsedCSV[csvID][7];
    std::string rightTexture     = parsedCSV[csvID][8];
    std::string leftTexture      = parsedCSV[csvID][9];
    glm::vec3 initPosition       = glm::vec3(std::stof(parsedCSV[csvID][10]), std::stof(parsedCSV[csvID][11]), std::stof(parsedCSV[csvID][12]));
    glm::vec3 initScale          = glm::vec3(std::stof(parsedCSV[csvID][13]), std::stof(parsedCSV[csvID][14]), std::stof(parsedCSV[csvID][15]));
    glm::vec4 initRotation       = glm::vec4(std::stof(parsedCSV[csvID][16]), std::stof(parsedCSV[csvID][17]), std::stof(parsedCSV[csvID][18]), std::stof(parsedCSV[csvID][19]));
    glm::vec4 color              = glm::vec4(std::stof(parsedCSV[csvID][28]), std::stof(parsedCSV[csvID][29]), std::stof(parsedCSV[csvID][30]), std::stof(parsedCSV[csvID][31]));
    glm::vec4 hitSphere          = glm::vec4(std::stof(parsedCSV[csvID][20]), std::stof(parsedCSV[csvID][21]), std::stof(parsedCSV[csvID][22]), std::stof(parsedCSV[csvID][23]));
    glm::vec4 moveSphere         = glm::vec4(std::stof(parsedCSV[csvID][24]), std::stof(parsedCSV[csvID][25]), std::stof(parsedCSV[csvID][26]), std::stof(parsedCSV[csvID][27]));
    ObjectStorage* objectStorage = this;
    if ((type == "object") || (type == "desk") || (type == "mark")) {
        Object3D newObject3D(id, name, type, meshID, textureID, iconID, initPosition, initScale, initRotation, color, hitSphere, moveSphere, objectStorage);
        object3Ds.insert(std::pair<int, Object3D>(id, newObject3D));
    }
    if ((type == "vehicle")) {
        Object3Dvehicle newObject3Dvehicle(id, name, type, meshID, textureID, lightTexture, breakTexture, rightTexture, leftTexture,
        iconID, initPosition, initScale, initRotation, color, hitSphere, moveSphere, objectStorage);
        object3Dvehicles.insert(std::pair<int, Object3Dvehicle>(id, newObject3Dvehicle));
    }
}

bool ObjectStorage::loadingCheck() {
    loadingState = SDL_AtomicGet(&atomicThreadCounter);
    return SDL_AtomicGet(&atomicThreadCounter) == threads.size();
}

void ObjectStorage::finaliseLoading() {
    for (size_t i = 0; i < threads.size(); i++) {
        threads[i].join();
    }
    SDL_Delay(100);
    bindObjects();
    bindTextures();
    loaded = true;
}

void ObjectStorage::loadTexture(std::string fileName) {
    texturesMutex.lock();
    Texture2D& texture = textures[fileName];
    texturesMutex.unlock();
    texture.FromFileParallel(textureFolder + fileName);
}

std::thread ObjectStorage::loadTextureParallel(std::string fileName) {
    return std::thread([=]{
        loadTexture(fileName);
        SDL_AtomicAdd(&atomicThreadCounter, 1);
        });
}

void ObjectStorage::bindTextures() {
    std::map<std::string, Texture2D>::iterator it;
    for (it = textures.begin(); it != textures.end(); it++) {
        it->second.bindTexture();
    }
}

void ObjectStorage::loadObject(std::string fileName) {
    objectsMutex.lock();
    std::unique_ptr<Mesh>& object = objects[fileName];
    objectsMutex.unlock();
    object = ObjParser::parse((modelFolder + fileName).c_str(), true);
}

std::thread ObjectStorage::loadObjectParallel(std::string fileName) {
    return std::thread([=] {
        loadObject(fileName);
        SDL_AtomicAdd(&atomicThreadCounter, 1);
        });
}

void ObjectStorage::bindObjects() {
    std::map<std::string, std::unique_ptr<Mesh>>::iterator it;
    for (it = objects.begin(); it != objects.end(); it++) {
        it->second->initBuffers();
    };
}

void ObjectStorage::readCSV() {
    std::ifstream file(configFile);
    std::string line;
    int lineCount = 0;
    while (std::getline(file, line)) {
        if (lineCount >= 2) {
            std::stringstream lineStream(line);
            std::string cell;
            std::vector<std::string> parsedRow;
            while (std::getline(lineStream, cell, ';')) {
                parsedRow.push_back(cell);
            }
            parsedCSV.push_back(parsedRow);
        }
        lineCount++;
    }
	file.close();
}

bool ObjectStorage::isThisTextureLoaded(std::string textureName) {
    return(textures.find(textureName) != textures.end());
}

bool ObjectStorage::isThisObjectLoaded(std::string objectName) {
    return(objects.find(objectName) != objects.end());
}