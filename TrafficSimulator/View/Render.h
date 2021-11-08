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
	 * @brief Add new renderable object from the preloaded object storage to the scene.
	 * @param id New object object storage ID.
	 * @return The new object render ID.
	*/
	int addObject(int objectStorageID);

	/**
	 * @brief Add dynamicly generated road to the scene.
	 * @return The new road dynamicRenderID.
	*/
	int addRoad();

	/**
	 * @brief Delete a scene object by renderID.
	 * @param id renderID
	*/
	void deleteObject(int dynamicRenderID);

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
	*/
	void rebindRoads();

	/**
	 * @brief Flagvariable for the first scene configs.
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
};