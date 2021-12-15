/**
 * @name Traffic Simulation
 * @file ObjectStorage.cpp
 * @class ObjectStorage
 * @author Sándor Balázs - AZA6NL
 * @date 2021.12.15.
 * @brief Load and store the 3D Models and Textures and create the render objects.
 * Contact: sandorbalazs9402@gmail.com
*/

#include "ObjectStorage.h"
#include <sstream>
#include <fstream>

/**
 * @brief Constructor, empty.
 * @param  Empty.
*/
ObjectStorage::ObjectStorage(void) {
}

/**
 * @brief Destructor, empty.
 * @param Empty.
*/
ObjectStorage::~ObjectStorage(void) {
}

/**
 * @brief Load the CSV configuration file and load the Objects and the textures parallel by the CSV configuration file.
*/
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

/**
 * @brief Create and store the renderable 3D object data.
 * @param csvID The processable row ID of the CSV configuration file.
*/
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

/**
 * @brief Check all the loader threads. Return true if all loader threads finished.
 * @return True if all loader threads finished.
*/
bool ObjectStorage::loadingCheck() {
    loadingState = SDL_AtomicGet(&atomicThreadCounter);
    return SDL_AtomicGet(&atomicThreadCounter) == threads.size();
}

/**
 * @brief Join all loading threads, bind the objects and textures.
*/
void ObjectStorage::finaliseLoading() {
    for (size_t i = 0; i < threads.size(); i++) {
        threads[i].join();
    }
    SDL_Delay(100);
    bindObjects();
    bindTextures();
    loaded = true;
}

/**
 * @brief Start load a texture in parallel thread safe.
 * @param fileName The loadable texture name.
*/
void ObjectStorage::loadTexture(std::string fileName) {
    texturesMutex.lock();
    Texture2D& texture = textures[fileName];
    texturesMutex.unlock();
    texture.FromFileParallel(textureFolder + fileName);
}

/**
 * @brief Load a texture in new thread.
 * @param fileName The loadable texture name.
 * @return The new thread.
*/
std::thread ObjectStorage::loadTextureParallel(std::string fileName) {
    return std::thread([=]{
        loadTexture(fileName);
        SDL_AtomicAdd(&atomicThreadCounter, 1);
        });
}

/**
 * @brief Bind loaded textures.
*/
void ObjectStorage::bindTextures() {
    std::map<std::string, Texture2D>::iterator it;
    for (it = textures.begin(); it != textures.end(); it++) {
        it->second.bindTexture();
    }
}

/**
 * @brief Start load a 3D Object in parallel thread safe.
 * @param fileName The loadable 3D Object name.
*/
void ObjectStorage::loadObject(std::string fileName) {
    objectsMutex.lock();
    std::unique_ptr<Mesh>& object = objects[fileName];
    objectsMutex.unlock();
    object = ObjParser::parse((modelFolder + fileName).c_str(), true);
}

/**
 * @brief Load a 3D Object in a new thread.
 * @param fileName The loadable 3D Object name.
*/
std::thread ObjectStorage::loadObjectParallel(std::string fileName) {
    return std::thread([=] {
        loadObject(fileName);
        SDL_AtomicAdd(&atomicThreadCounter, 1);
        });
}

/**
 * @brief Bind a loaded object.
*/
void ObjectStorage::bindObjects() {
    std::map<std::string, std::unique_ptr<Mesh>>::iterator it;
    for (it = objects.begin(); it != objects.end(); it++) {
        it->second->initBuffers();
    };
}

/**
 * @brief Read, process and store the configuration CSV file.
*/
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

/**
 * @brief Thread loading checker.
 * @param textureName The loadable texture.
 * @return True if the loadable texture is loaded.
*/
bool ObjectStorage::isThisTextureLoaded(std::string textureName) {
    return(textures.find(textureName) != textures.end());
}

/**
 * @brief Thread loading checker.
 * @param textureName The loadable 3D Object.
 * @return True if the loadable 3D Object is loaded.
*/
bool ObjectStorage::isThisObjectLoaded(std::string objectName) {
    return(objects.find(objectName) != objects.end());
}

/**
 * @brief Getter for the window icon.
 * @return The current window icon.
*/
SDL_Surface* ObjectStorage::getWindowIcon() {
    return windowIcon;
}

/**
 * @brief Getter for the textures.
 * @param textureName The needed texture name.
 * @return The needed texture.
*/
Texture2D& ObjectStorage::getTexture(std::string textureName) {
    if (isThisTextureLoaded(textureName)) {
        return textures[textureName];
    }
    else {
        return textures[defaultTexture];
    }
}

/**
 * @brief Getter for the Meshes.
 * @param textureName The needed Mesh name.
 * @return The needed Mesh.
*/
std::unique_ptr<Mesh>& ObjectStorage::getMesh(std::string meshName) {
    if (isThisObjectLoaded(meshName)) {
        return objects[meshName];
    }
    else {
        return objects[defaultObject];
    }
}

/**
 * @brief Loading finish checker.
 * @return Return true if the loading is finished.
*/
bool ObjectStorage::isLoaded() {
    return loaded;
}

/**
 * @brief Getter for the current state of the loading procedure.
 * @return The loading state of the loading procedure.
*/
int ObjectStorage::getLoadingState() {
    return loadingState;
}

/**
 * @brief Getter for the maximum of the loading state for detect the loading finish.
 * @return The maximum of the loading state.
*/
int ObjectStorage::getLoadingStateMax() {
    return loadingStateMax;
}

/**
 * @brief Create a new copy of the needed 3D object. If it not found give the first.
 * @param object3Did The needed 3D Object ID.
 * @return The copy of the needed 3D Object.
*/
Object3D  ObjectStorage::getObject3D(int object3Did) {
    auto object3D = object3Ds.find(object3Did);
    if (object3D == object3Ds.end()) {
        Logger::error("getObject3D error. Needed object not found: " + object3Did);
        return getObject3D(1);
    }
    else {
        return object3D->second.copy();
    }

}

/**
 * @brief Create a new copy of the needed 3D object and set its render id too. If it not found give the first.
 * @param object3Did The needed 3D Object ID.
 * @param renderID The needed 3D copy new render ID.
 * @return The copy of the needed 3D Object.
*/
Object3D  ObjectStorage::getObject3D(int object3Did, int renderID) {
    auto object3D = object3Ds.find(object3Did);
    if (object3D == object3Ds.end()) {
        Logger::error("getObject3D error. Needed object not found: " + object3Did);
        return getObject3D(1, renderID);
    }
    else {
        return object3D->second.copy(renderID);
    }
}

/**
 * @brief Create a new copy of the needed 3D vehicle object. If it not found give the first (5).
 * @param object3Did The needed 3D vehicle Object ID.
 * @return The copy of the needed 3D vehicle Object.
*/
Object3Dvehicle  ObjectStorage::getObject3Dvehicle(int object3Did) {
    auto object3Dvehicle = object3Dvehicles.find(object3Did);
    if (object3Dvehicle == object3Dvehicles.end()) {
        Logger::error("getObject3D error. Needed object not found: " + object3Did);
        return getObject3Dvehicle(5);
    }
    else {
        return object3Dvehicle->second.copy();
    }
}

/**
 * @brief Create a new copy of the needed 3D vehicle object and set its render id too. If it not found give the first (5).
 * @param object3Did The needed 3D vehicle Object ID.
 * @param renderID The needed 3D copy new render ID.
 * @return The copy of the needed 3D vehicle Object.
*/
Object3Dvehicle  ObjectStorage::getObject3Dvehicle(int object3Did, int renderID) {
    auto object3Dvehicle = object3Dvehicles.find(object3Did);
    if (object3Dvehicle == object3Dvehicles.end()) {
        Logger::error("getObject3D error. Needed object not found: " + object3Did);
        return getObject3Dvehicle(5, renderID);
    }
    else {
        return object3Dvehicle->second.copy(renderID);
    }
}

/**
 * @brief Get the first scene elements list.
 * @return The first scene elements list.
*/
std::vector<int>& ObjectStorage::getFirstSceneElements() {
    return firstSceneElements;
}