#pragma once
#include <vector>
#include <map>
#include <mutex>
#include <thread>
#include "../Utilities/ObjParser_OGL3.h"
#include "../Utilities/TextureObject.h"
#include "../Utilities/Mesh_OGL3.h"
#include "../Utilities/VertexArrayObject.h"
#include "Object3D.h"
#include "Object3Dvehicle.h"
#include <SDL.h>

class ObjectStorage {
public:
	ObjectStorage(void);
	~ObjectStorage(void);
	void load();
	SDL_Surface* getWindowIcon() {
		return windowIcon;
	}

	Texture2D& getTexture(std::string textureName) {
		if (isThisTextureLoaded(textureName)) {
			return textures[textureName];
		} else {
			return textures[defaultTexture];
		}
	}

	std::unique_ptr<Mesh>& getMesh(std::string meshName) {
		if (isThisObjectLoaded(meshName)) {
			return objects[meshName];
		}
		else {
			return objects[defaultObject];
		}
	}

	bool isLoaded() {
		return loaded;
	}

	bool loadingCheck();

	void finaliseLoading();

	int getLoadingState() {
		return loadingState;
	}

	int getLoadingStateMax() {
		return loadingStateMax;
	}

	//TODO error handling
	Object3D getObject3D(int object3Did) {
		auto object3D = object3Ds.find(object3Did);
		if (object3D == object3Ds.end()) {
			std::cout << "AJJJAJJJ!" << std::endl;
		} else {

		}
		return object3D->second.copy();
	}

	//TODO error handling
	Object3D getObject3D(int object3Did, int renderID) {
		auto object3D = object3Ds.find(object3Did);
		if (object3D == object3Ds.end()) {
			std::cout << "AJJJAJJJ!" << std::endl;
		} else {

		}
		return object3D->second.copy(renderID);
	}

	//TODO error handling
	Object3Dvehicle getObject3Dvehicle(int object3Did) {
		auto object3Dvehicle = object3Dvehicles.find(object3Did);
		if (object3Dvehicle == object3Dvehicles.end()) {
			std::cout << "AJJJAJJJ!" << std::endl;
		} else {

		}
		return object3Dvehicle->second.copy();
	}

	//TODO error handling
	Object3Dvehicle getObject3Dvehicle(int object3Did, int renderID) {
		auto object3Dvehicle = object3Dvehicles.find(object3Did);
		if (object3Dvehicle == object3Dvehicles.end()) {
			std::cout << "AJJJAJJJ!" << std::endl;
		} else {

		}
		return object3Dvehicle->second.copy(renderID);
	}

	std::vector<int>& getFirstSceneElements() {
		return firstSceneElements;
	}

	std::map<int, Object3D> object3Ds;

	std::map<int, Object3Dvehicle> object3Dvehicles;

private:
	const std::string defaultTexture = "default.png";
	const std::string defaultObject = "sphere_mark.obj";
	const std::string configFile = ".\\3Dobjects\\3d_object_library.csv";
	const std::string textureFolder = ".\\3Dobjects\\textures\\";
	const std::string miniatureFolder = "miniatures\\";
	const std::string modelFolder = ".\\3Dobjects\\models\\";

	bool loaded = false;
	int loadingState = 0;
	int loadingStateMax = 0;


	SDL_Surface* windowIcon;

	std::vector<std::vector<std::string>> parsedCSV;

	void readCSV();

	std::mutex texturesMutex;
	std::map<std::string, Texture2D> textures;
	void loadTexture(std::string fileName);
	bool isThisTextureLoaded(std::string textureName);
	std::thread loadTextureParallel(std::string fileName);
	void bindTextures();

	std::mutex objectsMutex;
	std::map<std::string, std::unique_ptr<Mesh>> objects;
	void loadObject(std::string fileName);
	bool isThisObjectLoaded(std::string objectName);
	std::thread loadObjectParallel(std::string fileName);
	void bindObjects();

	std::vector<std::thread> threads;
	SDL_atomic_t atomicThreadCounter;

	std::vector<int> firstSceneElements;

	//std::map<int, Object3D> object3Ds;

	std::map<std::string, VertexArrayObject> generatedObjects;

	void store(int csvID);
};