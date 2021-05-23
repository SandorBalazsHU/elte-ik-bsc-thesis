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

	std::unique_ptr<Mesh>& getObject(std::string textureName) {
		if (isThisObjectLoaded(textureName)) {
			return objects[textureName];
		}
		else {
			return objects[defaultObject];
		}
	}

private:
	const std::string defaultTexture = "default.png";
	const std::string defaultObject = "sphere_mark.obj";
	const std::string configFile = "3Dobjects/3d_object_library.csv";
	const std::string textureFolder = "3Dobjects/textures/";
	const std::string modelFolder = "3Dobjects/models/";

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


	std::map<std::string, VertexArrayObject> generatedObjects;

	std::map<std::string, Object3D> Object3Ds;

};