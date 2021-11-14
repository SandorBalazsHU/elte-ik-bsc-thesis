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
#include "Utilities/ProgramObject.h"
#include "Utilities/TextureObject.h"
#include "Utilities/Mesh_OGL3.h"
#include "Utilities/VertexArrayObject.h"
#include "Objects/ObjectStorage.h"
#include "Objects/Object3Droad.h"
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
	 * @brief Set the texture for the object rendering.
	 * @param texture The usable texture.
	*/
	void setTexture(Texture2D& texture);

	/**
	 * @brief Remder the next frame. Use the shaders, update the camera and the gui. Render the objects and the curent scene.
	*/
	void render();

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
	 * @brief Render mode configurator - Wireframe object drawing on.
	*/
	void wireframeOn();

	/**
	* @brief Render mode configurator - Wireframe object drawing off.
	*/
	void wireframeOff();

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
	 * @brief Add dynamicly generated road to the scene.
	 * /attention The road objects have 4 marker and 2 filler object for dependencies.
	 * @return The new road dynamicRenderID.
	*/
	int addRoad();

	/**
	 * @brief Delete a scene object by renderID.
	 * /attention This cause pointer invalidation in the vector. Use the updateRenderIDs and rebindRoads for repairing pointers.
	 * @param id renderID
	*/
	void deleteObject(int dynamicRenderID);

	/**
	 * @brief Exit program.
	*/
	void close();

	/**
	 * @brief Update a dynamicly generated object status by dynamicRenderID.
	 * @param The updatable dynamicRenderID 
	*/
	void updateDynamicObject(int dynamicRenderID);

	/**
	 * @brief Get scene preloaded object from the scene render storage.
	 * @param id renderID
	 * @return The scene object.
	*/
	Object3D* getObject(int renderID);

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
	 * @brief List of the renderable scene objets. (Preloaded meshes)
	*/
	std::vector<Object3D> renderableObjects;

	/**
	 * @brief List of the renderable scene roads. (Dynamicly generated shapes)
	*/
	std::vector<Object3Droad*> renderableRoads;

private:
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
	 * @brief Refresh the renderable objects renderID-s after modiflying the renderable objects list.
	*/
	void updateRenderIDs();

	/**
	* @brief Refresh and rebind the renderable roads (Dynamic generated object) renderID-s and byndings after modiflying the renderable objects list.
	* The main goal is repair the pointer invalidation after object delete.
	* Scan over the all the objects with dependencies and and rebind the six dependecies of all roads.
	*/
	void rebindRoads();

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

	/**
	 * @brief Flagvariable for the first scene configs. 
	 * /attention Default value is TRUE.
	*/
	bool firstScreen = true;

	/**
	 * @brief Hide the desk for object photo mode. (Debug function)
	*/
	bool objectPhotoMode = false;

	/**
	 * @brief Active the hit sphare drawing. (DebugFunction)
	*/
	bool hitSphare = false;

	/**
	 * @brief Active the road hit sphare drawing. (DebugFunction)
	*/
	bool roadHhitSphare = false;

	/**
	 * @brief Active the wireframe function for the objets. (DebugFunction)
	*/
	bool objectsWireframe = false;

	/**
	 * @brief Active the wireframe function for the roads. (DebugFunction)
	*/
	bool roadWireframe = false;

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
};