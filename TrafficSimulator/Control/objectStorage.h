#pragma once
#include <vector>
#include <map>
#include <mutex>
#include <thread>
#include "../View/Utilities/ProgramObject.h"
#include "../View/Utilities/TextureObject.h"
#include "../View/Utilities/Mesh_OGL3.h"
#include "../View/Utilities/VertexArrayObject.h"

class objectStorage {
public:
	objectStorage(void);
	~objectStorage(void);
	void load();
	void loadTexture(std::string fileName);
	std::thread loadTextureParallel(std::string fileName);


	std::mutex texturesMutex;
	std::map<std::string, Texture2D> textures;
	std::mutex objectsMutex;
	std::map<std::string, std::unique_ptr<Mesh>> objects;
	std::map<std::string, VertexArrayObject> generatedObjects;
	SDL_Surface* getWindowIcon() {
		return windowIcon;
	}

private:
	SDL_Surface* windowIcon;
	const std::string configFile = "3Dobjects/3d_object_library.csv";
	const std::string textureFolder = "3Dobjects/textures/";
	bool isThisTextureLoaded(std::string textureName);
	std::vector<std::vector<std::string>> parsedCSV;
	void readCSV();
};