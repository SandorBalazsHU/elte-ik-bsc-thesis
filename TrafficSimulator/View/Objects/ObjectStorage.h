/**
 * @name Traffic Simulation
 * @file ObjectStorage.h
 * @class ObjectStorage
 * @author Sándor Balázs - AZA6NL
 * @date 2021.12.15.
 * @brief Load and store the 3D Models and Textures and create the render objects.
 * Contact: sandorbalazs9402@gmail.com
*/

#pragma once

#include <vector>
#include <map>
#include <mutex>
#include <thread>
#include <SDL.h>
#include "../../Control/Logger.h"
#include "../Utilities/ObjParser_OGL3.h"
#include "../Utilities/TextureObject.h"
#include "../Utilities/Mesh_OGL3.h"
#include "../Utilities/VertexArrayObject.h"
#include "Object3D.h"
#include "Object3Dvehicle.h"

/**
 * @brief Load and store the 3D Models and Textures and create the render objects.
*/
class ObjectStorage {
public:

	/**
	 * @brief Constructor, empty.
	 * @param  Empty.
	*/
	ObjectStorage(void);

	/**
	 * @brief Destructor, empty.
	 * @param Empty.
	*/
	~ObjectStorage(void);

	/**
	 * @brief Load the CSV configuration file and load the Objects and the textures parallel by the CSV configuration file.
	*/
	void load();

	/**
	 * @brief Getter for the window icon.
	 * @return The current window icon.
	*/
	SDL_Surface* getWindowIcon();

	/**
	 * @brief Getter for the textures.
	 * @param textureName The needed texture name.
	 * @return The needed texture.
	*/
	Texture2D& getTexture(std::string textureName);

	/**
	 * @brief Getter for the Meshes.
	 * @param textureName The needed Mesh name.
	 * @return The needed Mesh.
	*/
	std::unique_ptr<Mesh>& getMesh(std::string meshName);

	/**
	 * @brief Loading finish checker.
	 * @return Return true if the loading is finished.
	*/
	bool isLoaded();

	/**
	 * @brief Check all the loader threads. Return true if all loader threads finished.
	 * @return True if all loader threads finished.
	*/
	bool loadingCheck();

	/**
	 * @brief Join all loading threads, bind the objects and textures.
	*/
	void finaliseLoading();

	/**
	 * @brief Getter for the current state of the loading procedure.
	 * @return The loading state of the loading procedure.
	*/
	int getLoadingState();

	/**
	 * @brief Getter for the maximum of the loading state for detect the loading finish.
	 * @return The maximum of the loading state.
	*/
	int getLoadingStateMax();

	/**
	 * @brief Create a new copy of the needed 3D object. If it not found give the first.
	 * @param object3Did The needed 3D Object ID.
	 * @return The copy of the needed 3D Object.
	*/
	Object3D getObject3D(int object3Did);

	/**
	 * @brief Create a new copy of the needed 3D object and set its render id too. If it not found give the first.
	 * @param object3Did The needed 3D Object ID.
	 * @param renderID The needed 3D copy new render ID.
	 * @return The copy of the needed 3D Object.
	*/
	Object3D getObject3D(int object3Did, int renderID);

	/**
	 * @brief Create a new copy of the needed 3D vehicle object. If it not found give the first (5).
	 * @param object3Did The needed 3D vehicle Object ID.
	 * @return The copy of the needed 3D vehicle Object.
	*/
	Object3Dvehicle getObject3Dvehicle(int object3Did);

	/**
	 * @brief Create a new copy of the needed 3D vehicle object and set its render id too. If it not found give the first (5).
	 * @param object3Did The needed 3D vehicle Object ID.
	 * @param renderID The needed 3D copy new render ID.
	 * @return The copy of the needed 3D vehicle Object.
	*/
	Object3Dvehicle getObject3Dvehicle(int object3Did, int renderID);

	/**
	 * @brief Get the first scene elements list.
	 * @return The first scene elements list.
	*/
	std::vector<int>& getFirstSceneElements();

	/**
	 * @brief The original 3D objects storage with the original configurations.
	*/
	std::map<int, Object3D> object3Ds;

	/**
	 * @brief The original 3D vehicle objects storage with the original configurations.
	*/
	std::map<int, Object3Dvehicle> object3Dvehicles;

private:
	const std::string defaultTexture = "default.png";
	const std::string defaultObject = "sphere_mark.obj";
	const std::string configFile = "3Dobjects/3d_object_library.csv";
	const std::string textureFolder = "3Dobjects/textures/";
	const std::string miniatureFolder = "miniatures/";
	const std::string modelFolder = "3Dobjects/models/";

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

	std::map<std::string, VertexArrayObject> generatedObjects;

	void store(int csvID);
};