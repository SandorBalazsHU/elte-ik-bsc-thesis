/**
 * @name Traffic Simulation
 * @file Render.h
 * @class Render
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief The 3D Scene render class
 * Contact: sandorbalazs9402@gmail.com
*/

#pragma once
/**
 * @brief Empty classes for prevent circular including. 
*/
class WorkWindow;
class GUI;

#include "Camera.h"
#include "Animator.h"
#include "Utilities/ProgramObject.h"
#include "Utilities/TextureObject.h"
#include "Utilities/Mesh_OGL3.h"
#include "Utilities/VertexArrayObject.h"
#include "Objects/ObjectStorage.h"
#include "Objects/Object3Droad.h"
#include "Objects/object3Dvehicle.h"
#include "../Control/MapLoader.h"
#include "../Control/MapSaver.h"

/**
 * @brief The 3D Scene render implementation.
*/
class Render {
public:
	/**
	 * @brief Render constructor.
	*/
	Render(void);

	/**
	* Render destructor.
	*/
	~Render(void);

	/**
	 * @brief Binding the render to the window.
	 * @param window The joinable window.
	*/
	void bind(WorkWindow* window);

	/**
	 * @brief Remder the next frame. Use the shaders, update the camera and the gui. Render the objects and the curent scene.
	*/
	void render();

	/**
	 * @brief Render mode configurator - Vsync on.
	*/
	void vsyncOn();

	/**
	 * @brief Render mode configurator - Vsync off.
	*/
	void vsyncOff();

	/**
	 * @brief Render mode configurator - Multisampling on.
	*/
	void multisamplingOn();

	/**
	 * @brief Render mode configurator - Multisampling off.
	*/
	void multisamplingOff();

	/**
	* @brief Clearing the scene.
	*/
	void clearScrean();

	/**
	 * @brief Clean and renew the map editor.
	*/
	void clear();

	/**
	 * @brief Add new renderable object from the preloaded object storage to the scene.
	 * @param id New object object storage ID.
	 * @return The new object render ID.
	*/
	int addObject(int objectStorageID);

	/**
	 * @brief Delete a scene object by renderID.
	 * /attention This cause pointer invalidation in the vector. Use the updateRenderIDs and rebindRoads for repairing pointers.
	 * @param renderID renderID
	*/
	void deleteObject(int renderID);

	/**
	 * @brief Add dynamicly generated road to the scene.
	 * /attention The road objects have 4 marker and 2 filler object for dependencies.
	 * @return The new road dynamicRenderID.
	*/
	int addRoad();

	/**
	 * @brief Delete a scene road object by dynamicRenderID.
	 * /attention This cause pointer invalidation in the vector. Use the updateRenderIDs and rebindRoads for repairing pointers.
	 * @param dynamicRenderID dynamicRenderID for the renderable roads.
	*/
	void deleteRoad(int dynamicRenderID);

	/**
	 * @brief Add a new vehicle to the scene, from the objectStorage.
	 * @param objectStorageID The addable vehicle object storage ID.
	 * @return The added vehicle renderID.
	*/
	size_t addVehicle(int objectStorageID);

	/**
	 * @brief Delete a vehicle by renderID.
	 * @param renderableVehicleID The deletable vehicle renderID.
	*/
	void deleteVehicle(size_t renderableVehicleID);

	/**
	 * @brief Exit program.
	*/
	void close();

	/**
	 * @brief Update a dynamicly generated object status by dynamicRenderID.
	 * @param The updatable dynamicRenderID 
	*/
	void updateDynamicObject(size_t dynamicRenderID);

	/**
	 * @brief Getter for the map loader program.
	 * @return pointer for the map loader instance.
	*/
	MapLoader* getMapLoader();

	/**
	 * @brief Getter for the map saver program.
	 * @return Pointer for the map loader instance.
	*/
	MapSaver* getMapSaver();

	/**
	 * @brief Getter for the GUI.
	 * @return The current GUI.
	*/
	GUI* getGui();

	/**
	 * @brief Get scene preloaded object from the scene render storage.
	 * @param id renderID
	 * @return The scene object.
	*/
	Object3D* getObject(int renderID);

	/**
	 * @brief Getter for the RenderableRoads size.
	 * @return The renderableRoads size.
	*/
	size_t getObjectsNumber();

	/**
	 * @brief Getter for the RenderableRoads.
	 * @param renderID The needed object dynamicRenderID.
	 * @return The needed object.
	*/
	Object3Droad* getDynamicObject(size_t dynamicRenderID);

	/**
	 * @brief Getter for the renderableRoads size.
	 * @return The renderableRoads size.
	*/
	size_t getDynamicObjectsNumber();

	/**
	 * @brief Getter for the renderableVehicles;
	 * @param renderableVehicleID The needed vehicle's id.
	 * @return The needed vehicle.
	*/
	Object3Dvehicle* getVehicle(size_t renderableVehicleID);

	/**
	 * @brief Getter for the renderableVehicles size.
	 * @return The renderableVehicles size.
	*/
	size_t getVehiclesNumber();

	/**
	 * @brief Hide the desk for object photo mode. Function on.(Debug function)
	*/
	void photoModeOn();

	/**
	 * @brief Hide the desk for object photo mode. Function off. (Debug function)
	*/
	void photoModeOff();

	/**
	 * @brief Active the hit sphare drawing. (DebugFunction)
	*/
	bool hitSphare = false;

	/**
	 * @brief Active the road hit sphare drawing. (DebugFunction)
	*/
	bool roadHhitSphare = false;

	/**
	 * @brief Show the road hit spheres. (DebugFunction)
	*/
	bool showRoadHitSphereMiddle = true;

	/**
	 * @brief Show road side 1 markers. (DebugFunction)
	*/
	bool showRoadSide01 = false;

	/**
	 * @brief Show road side 2 markers. (DebugFunction)
	*/
	bool showRoadSide02 = false;

	/**
	 * @brief Show road track 1 markers. (DebugFunction)
	*/
	bool showRoadTrack01 = false;

	/**
	 * @brief Show road track 2 markers. (DebugFunction)
	*/
	bool showRoadTrack02 = false;

	/**
	 * @brief Active the wireframe function for the objets. (DebugFunction)
	*/
	bool objectsWireframe = false;

	/**
	 * @brief Active the wireframe function for the roads. (DebugFunction)
	*/
	bool roadWireframe = false;

	/**
	 * @brief The render's animator instance.
	*/
	Animator animator;


private:
	/**
	 * @brief List of the renderable scene objets. (Preloaded meshes)
	*/
	std::vector<Object3D> renderableObjects;

	/**
	 * @brief List of the renderable scene roads. (Dynamicly generated shapes)
	*/
	std::vector<Object3Droad*> renderableRoads;

	/**
	 * @brief The lost of renderable vehicle.
	*/
	std::vector<Object3Dvehicle> renderableVehicles;

	/**
	 * @brief Flagvariable for the first scene configs. 
	 * /attention Default value is TRUE.
	*/
	bool firstScreen = true;

	/**
	 * @brief The access pointer for the current working window object.
	*/
	WorkWindow* workingWindow;

	/**
	 * @brief The access pointer for the current window object.
	*/
	SDL_Window* window;

	/**
	 * @brief The access pointer for the current camera object.
	*/
	Camera* camera;

	/**
	 * @brief The access pointer for the current shader program.
	*/
	ProgramObject* shader;

	/**
	 * @brief The access pointer for the current GUI object.
	*/
	GUI* gui;

	/**
	 * @brief The access pointer for the current object storage (This store the preloaded meshes and textures, allways in one instance.).
	*/
	ObjectStorage* objectStorage;

	/**
	 * @brief The map loader class.
	*/
	MapLoader mapLoader;

	/**
	 * @brief The map saver class.
	*/
	MapSaver mapSaver;

	/**
	 * @brief Set the texture for the object rendering.
	 * @param texture The usable texture.
	*/
	void setTexture(Texture2D& texture);

	/**
	 * @brief Upload the shader datas before rendering an object.
	 * @param worldMatrix The world matrix for the shader.
	 * @param color The rendering RGBA color.
	*/
	void shaderPreDrawingUpdate(glm::mat4 worldMatrix, glm::vec4 color);

	/**
	 * @brief Drawing a file preloaded mesh.
	 * @param mesh The preloaded mesh for drawing.
	*/
	void drawMesh(std::unique_ptr<Mesh>& mesh);

	/**
	 * @brief Draving a dynamicly generated object.
	 * @param vao The dynamicly generated object for drawing.
	 * @param trianglesNumber The drawable object triangle number.
	*/
	void drawVao(VertexArrayObject& vao, int trianglesNumber);

	/**
	 * @brief Swapping the OpenGL drawing and showed image buffers. This is the true drawing.
	*/
	void rendering();

	/**
	 * @brief Windowe title text setter.
	 * @param title The new window title.
	*/
	void setWindowTitle(std::string title);

	/**
	 * @brief Set the window icon.
	 * @param windowIcon The new window icon.
	*/
	void setWindowIcon(SDL_Surface* windowIcon);

	/**
	 * @brief Render mode configurator - Face test on.
	*/
	void faceTestOn();

	/**
	* @brief Render mode configurator - Face test off.
	*/
	void faceTestOff();

	/**
	 * @brief Camera position updating in the shadervariable.
	*/
	void shaderCameraUpdate();

	/**
	 * @brief Composing the renderable scene from the renderableObjects and the renderableRoads storages.
	*/
	void renderScrean();

	/**
	 * @brief Initialise the scene. Render the first objects.
	*/
	void sceneInit();

	/**
	 * @brief Render mode configurator - Wireframe object drawing on.
	*/
	void wireframeOn();

	/**
	* @brief Render mode configurator - Wireframe object drawing off.
	*/
	void wireframeOff();

	/**
	 * @brief Render the hit sphares for each scene objets. (Debug function)
	 * @param renderID The actual object render ID.
	*/
	void showHitSphere(int renderID);

	/**
	 * @brief Render the hit sphares for each scene road objets. (Debug function)
	 * @param roadPoints The drawable points
	 * @param size the marker size
	 * @param vec3 the marker colors
	*/
	void showRoadHitSphere(std::vector<glm::vec3>& roadPoints, float size, glm::vec3 color);

	/**
	 * @brief Render the hit sphares for each scene road objets. (Debug function)
	*/
	void showAllRoadHitSpheres();
};