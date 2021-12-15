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

	/**
	 * @brief The default texture. (Use if the needed is not available because some reason.)
	*/
	const std::string defaultTexture = "default.png";

	/**
	 * @brief The default 3d Object. (Use if the needed is not available because some reason.)
	*/
	const std::string defaultObject = "sphere_mark.obj";

	/**
	 * @brief The configuration CSV file path.
	*/
	const std::string configFile = "3Dobjects/3d_object_library.csv";

	/**
	 * @brief The texture storage folder path.
	*/
	const std::string textureFolder = "3Dobjects/textures/";

	/**
	 * @brief The miniatures storage folder path.
	*/
	const std::string miniatureFolder = "miniatures/";

	/**
	 * @brief The 3D Objects storage folder path.
	*/
	const std::string modelFolder = "3Dobjects/models/";

	/**
	 * @brief Loading finish flag.
	*/
	bool loaded = false;

	/**
	 * @brief The current loading state.
	*/
	int loadingState = 0;

	/**
	 * @brief The maximum of the loading state.
	*/
	int loadingStateMax = 0;

	/**
	 * @brief The loaded, parsed configuration CSV file.
	*/
	std::vector<std::vector<std::string>> parsedCSV;

	/**
	 * @brief Mutex for the parallel texture loading thread safety.
	*/
	std::mutex texturesMutex;

	/**
	 * @brief Mutex for the parallel 3D Object loading thread safety.
	*/
	std::mutex objectsMutex;

	/**
	 * @brief The texture objects storage. All texture is loaded to the memory only once and all object use these.
	*/
	std::map<std::string, Texture2D> textures;

	/**
	 * @brief The 3D Objects storage. All 3D Objet is loaded to the memory only once and all object use these.
	*/
	std::map<std::string, std::unique_ptr<Mesh>> objects;

	/**
	 * @brief The current used window icon.
	*/
	SDL_Surface* windowIcon;

	/**
	 * @brief The loading threads storage.
	*/
	std::vector<std::thread> threads;

	/**
	 * @brief The thread safe thread counter.
	*/
	SDL_atomic_t atomicThreadCounter;

	/**
	 * @brief List of the first scene elements.
	*/
	std::vector<int> firstSceneElements;

	/**
	 * @brief The storage of the dynamically generated objects.
	*/
	std::map<std::string, VertexArrayObject> generatedObjects;

	/**
	 * @brief Read, process and store the configuration CSV file.
	*/
	void readCSV();

	/**
	 * @brief Start load a texture in parallel thread safe.
	 * @param fileName The loadable texture name.
	*/
	void loadTexture(std::string fileName);

	/**
	 * @brief Thread loading checker.
	 * @param textureName The loadable texture.
	 * @return True if the loadable texture is loaded.
	*/
	bool isThisTextureLoaded(std::string textureName);

	/**
	 * @brief Load a texture in new thread.
	 * @param fileName The loadable texture name.
	 * @return The new thread.
	*/
	std::thread loadTextureParallel(std::string fileName);

	/**
	 * @brief Bind loaded textures.
	*/
	void bindTextures();

	/**
	 * @brief Start load a 3D Object in parallel thread safe.
	 * @param fileName The loadable 3D Object name.
	*/
	void loadObject(std::string fileName);

	/**
	 * @brief Thread loading checker.
	 * @param textureName The loadable 3D Object.
	 * @return True if the loadable 3D Object is loaded.
	*/
	bool isThisObjectLoaded(std::string objectName);

	/**
	 * @brief Load a 3D Object in a new thread.
	 * @param fileName The loadable 3D Object name.
	*/
	std::thread loadObjectParallel(std::string fileName);

	/**
	 * @brief Bind a loaded object.
	*/
	void bindObjects();

	/**
	 * @brief Create and store the renderable 3D object data.
	 * @param csvID The processable row ID of the CSV configuration file.
	*/
	void store(int csvID);
};