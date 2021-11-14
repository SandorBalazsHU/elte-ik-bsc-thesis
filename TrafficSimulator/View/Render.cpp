/**
 * @name Traffic Simulation
 * @file Render.h
 * @class Render
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief The 3D Scene render class
 * Contact: sandorbalazs9402@gmail.com
 * AX400
*/

#include "Render.h"
#include "WorkWindow.h"
#include "GUI.h"
#include "fpsCounter.h"

#include "Utilities/ObjParser_OGL3.h"
#include <glm/gtx/transform2.hpp>

#include <SDL.h>
#include <iostream>
#include <sstream>
#include "../Control/Logger.h"

/**
 * @brief Render constructor.
*/
Render::Render(void) {
}

/**
* Render destructor.
*/
Render::~Render(void) {
}

/**
 * @brief Binding the render to the window.
 * @param window The joinable window.
*/
void Render::bind(WorkWindow* currentWindow) {
	workingWindow = currentWindow;
	camera = workingWindow->getCamera();
	shader = workingWindow->getShader();
	window = workingWindow->getWindow();
	gui = workingWindow->getGUI();
	objectStorage = workingWindow->getObjectStorage();
	setWindowIcon(objectStorage->getWindowIcon());
	fpsCounter::bind();
	mapLoader.bind(this);
	mapSaver.bind(this);
	//TODO GUI Graphical settings
	//fpsCounter::fpsLimiterOn();
	//fpsCounter::setFpsLimit(45);
	multisamplingOn();
	vsyncOn();
}

/**
 * @brief Exit program.
*/
void Render::close() {
	workingWindow->close();
}

/**
 * @brief Render mode configurator - Multisampling on.
*/
void Render::multisamplingOn() {
	glEnable(GL_MULTISAMPLE);
}

/**
 * @brief Render mode configurator - Multisampling off.
*/
void Render::multisamplingOff() {
	glDisable(GL_MULTISAMPLE);
}

/**
 * @brief Render mode configurator - Vsync on.
*/
void Render::vsyncOn() {
	SDL_GL_SetSwapInterval(1);
}

/**
 * @brief Render mode configurator - Vsync off.
*/
void Render::vsyncOff() {
	SDL_GL_SetSwapInterval(0);
}

/**
 * @brief Render mode configurator - Wireframe object drawing on.
*/
void  Render::wireframeOn() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

/**
* @brief Render mode configurator - Wireframe object drawing off.
*/
void  Render::wireframeOff() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

/**
 * @brief Render mode configurator - Face test on.
*/
void Render::faceTestOn() {
	glEnable(GL_CULL_FACE);
}

/**
 * @brief Render mode configurator - Face test off.
*/
void Render::faceTestOff() {
	glDisable(GL_CULL_FACE);
}

/**
* @brief Clearing the scene.
*/
void Render::clearScrean() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * @brief Set the texture for the object rendering.
 * @param texture The usable texture.
*/
void Render::setTexture(Texture2D& texture) {
	shader->SetTexture("currentTexture", 0, texture);
}

/**
 * @brief Camera position updating in the shadervariable.
*/
void Render::shaderCameraUpdate() {
	shader->SetUniform("cameraPosition", camera->getCameraPosition());
}

/**
 * @brief Windowe title text setter.
 * @param title The new window title.
*/
void Render::setWindowTitle(std::string title) {
	std::stringstream window_title;
	window_title << title << "  -  " << fpsCounter::getAverageFPS() << " fps";
	SDL_SetWindowTitle(window, window_title.str().c_str());
}

/**
 * @brief Getter for the map loader program.
 * @return pointer for the map loader instance.
*/
MapLoader* Render::getMapLoader() {
	return &mapLoader;
}

/**
 * @brief Getter for the map saver program.
 * @return Pointer for the map loader instance.
*/
MapSaver* Render::getMapSaver() {
	return &mapSaver;
}

/**
 * @brief Upload the shader datas before rendering an object.
 * @param worldMatrix The world matrix for the shader.
 * @param color The rendering RGBA color.
*/
void Render::shaderPreDrawingUpdate(glm::mat4 worldMatrix, glm::vec4 color) {
	shader->SetUniform("worldMatrix", worldMatrix);
	shader->SetUniform("worldInverseTransposeMatrix", glm::transpose(glm::inverse(worldMatrix)));
	shader->SetUniform("projectionViewWorldMatrix", camera->getProjectionViewMatrix() * worldMatrix);
	shader->SetUniform("diffuseMaterial", color);
}

/**
 * @brief Drawing a file preloaded mesh.
 * @param mesh The preloaded mesh for drawing.
*/
void Render::drawMesh(std::unique_ptr<Mesh>& mesh) {
	mesh->draw();
}

/**
 * @brief Draving a dynamicly generated object.
 * @param vao The dynamicly generated object for drawing.
 * @param trianglesNumber The drawable object triangle number.
*/
void Render::drawVao(VertexArrayObject& vao, int trianglesNumber) {
	vao.Bind();
	glDrawElements(GL_TRIANGLES, trianglesNumber, GL_UNSIGNED_INT, 0);
	vao.Unbind();
}

/**
 * @brief Set the window icon.
 * @param windowIcon The new window icon.
*/
void Render::setWindowIcon(SDL_Surface* windowIcon) {
	SDL_SetWindowIcon(window, windowIcon);
}

/**
 * @brief Swapping the OpenGL drawing and showed image buffers. This is the true drawing.
*/
void Render::rendering() {
	SDL_GL_SwapWindow(window);
}

/**
 * @brief Initialise the scene. Render the first objects.
*/
void Render::sceneInit() {
	std::vector<int>& firstSceneElements = objectStorage->getFirstSceneElements();
	for (size_t i = 0; i < firstSceneElements.size(); i++) {
		addObject(firstSceneElements[i]);
	}
	if(objectPhotoMode) getObject(0)->hide();
}

/**
 * @brief Clean and renew the map editor.
*/
void Render::clear() {
	mapSaver.reset();
	renderableObjects.clear();
	renderableRoads.clear();
	sceneInit();
}

/**
 * @brief Add new renderable object from the preloaded object storage to the scene.
 * @param id New object object storage ID.
 * @return The new object render ID.
*/
int Render::addObject(int id) {
	renderableObjects.push_back(objectStorage->getObject3D(id, renderableObjects.size()));
	return renderableObjects.size()-1;
}

/**
 * @brief Delete a scene object by renderID.
 * \attention This cause pointer invalidation in the vector. Use the updateRenderIDs and rebindRoads for repairing pointers.
 * @param id renderID
*/
void Render::deleteObject(int renderID) {
	if (!renderableObjects[renderID].isProtected()) {
		renderableObjects.erase(renderableObjects.begin() + renderID);
		updateRenderIDs();
		rebindRoads();
	}
}

/**
 * @brief Refresh the renderable objects renderID-s after modiflying the renderable objects list.
 * The main goal is repair the pointer invalidation after object delete.
*/
void Render::updateRenderIDs() {
	for (size_t i = 0; i < renderableObjects.size(); i++) {
		renderableObjects[i].setRenderID(i);
	}
}

/**
* @brief Refresh and rebind the renderable roads (Dynamic generated object) renderID-s and byndings after modiflying the renderable objects list.
* The main goal is repair the pointer invalidation after object delete.
* Scan over the all the objects with dependencies and and rebind the six dependecies of all roads.
*/
void Render::rebindRoads() {
	if (renderableRoads.size() > 0) {
		std::vector<Object3D*> rebindableObjects;
		for (size_t i = 0; i < renderableObjects.size(); i++) {
			if (renderableObjects[i].getDependencyID() > -1) {
				rebindableObjects.push_back(&renderableObjects[i]);
			}
		}
		for (size_t i = 0; i < rebindableObjects.size(); i += 6) {
			renderableRoads[rebindableObjects[i]->getDependencyID()]->reBind(rebindableObjects[i], rebindableObjects[i + 1], rebindableObjects[i + 2], rebindableObjects[i + 3], rebindableObjects[i + 4], rebindableObjects[i + 5]);
		}
	}
}

/**
 * @brief Add dynamicly generated road to the scene.
 * /attention The road objects have 4 marker and 2 filler object for dependencies.
 * @return The new road dynamicRenderID.
*/
int Render::addRoad() {
	Object3Droad* road = new Object3Droad();
	addObject(1);
	addObject(1);
	addObject(1);
	addObject(1);
	addObject(2);
	addObject(2);

	Object3D* trackBall_0		= getObject(renderableObjects.size() - 6);
	Object3D* trackBall_1		= getObject(renderableObjects.size() - 5);
	Object3D* trackBall_2		= getObject(renderableObjects.size() - 4);
	Object3D* trackBall_3		= getObject(renderableObjects.size() - 3);
	Object3D* roadEndCircle_2	= getObject(renderableObjects.size() - 2);
	Object3D* roadEndCircle_1	= getObject(renderableObjects.size() - 1);

	trackBall_0->setDependencyID(renderableRoads.size());
	trackBall_1->setDependencyID(renderableRoads.size());
	trackBall_2->setDependencyID(renderableRoads.size());
	trackBall_3->setDependencyID(renderableRoads.size());
	roadEndCircle_2->setDependencyID(renderableRoads.size());
	roadEndCircle_1->setDependencyID(renderableRoads.size());

	road->bind(trackBall_0, trackBall_1, trackBall_2, trackBall_3, roadEndCircle_1, roadEndCircle_2);
	renderableRoads.push_back(road);

	rebindRoads();

	road->setRenderID(renderableRoads.size());

	return renderableRoads.size() - 1;
}

/**
 * @brief Get scene preloaded object from the scene render storage.
 * @param id renderID
 * @return The scene object.
*/
Object3D* Render::getObject(int id) {
	return &renderableObjects[id];
}

/**
 * @brief Update a dynamicly generated object status by dynamicRenderID.
 * @param The updatable dynamicRenderID
*/
void Render::updateDynamicObject(int id) {
	renderableRoads[id]->update();
	for (size_t i = 0; i < renderableRoads.size(); i++) {
		if (i != id) {
			renderableRoads[id]->stuckTest(renderableRoads[i]);
		}
	}
	for (size_t i = 0; i < renderableObjects.size(); i++) {
		if (renderableObjects[i].getName() == "Start sign" || renderableObjects[i].getName() == "Stop sign") {
			renderableRoads[id]->markerTest(&renderableObjects[i]);
		}
	}
}

/**
 * @brief Render the hit sphares for each scene objets. (Debug function)
*/
void Render::showHitSphere(int renderID) {
	Object3D hitSphere = objectStorage->getObject3D(1);
	hitSphere.setPosition(glm::vec3(renderableObjects[renderID].getHitSphere().x,
		renderableObjects[renderID].getHitSphere().y,
		renderableObjects[renderID].getHitSphere().z));
	hitSphere.setScale(glm::vec3(renderableObjects[renderID].getHitSphere().w,
		renderableObjects[renderID].getHitSphere().w,
		renderableObjects[renderID].getHitSphere().w));
	hitSphere.setOpacity(0.2f);

	setTexture(hitSphere.getTexture());
	shaderPreDrawingUpdate(hitSphere.getWorldMatrix(), hitSphere.getRGBAcolor());
	drawMesh(hitSphere.getMesh());
}

/**
 * @brief Render the hit sphares for each scene road objets. (Debug function)
*/
void Render::showRoadHitSphere(std::vector<glm::vec3>& roadPoints, float size, glm::vec3 color) {
	for (size_t i = 0; i < roadPoints.size(); i += 1) {
		Object3D hitSphere = objectStorage->getObject3D(1);
		hitSphere.setPosition(roadPoints[i]);
		hitSphere.setScale(glm::vec3(size, size, size));
		hitSphere.setRGBcolor(color);
		hitSphere.setOpacity(0.5f);
		setTexture(hitSphere.getTexture());
		shaderPreDrawingUpdate(hitSphere.getWorldMatrix(), hitSphere.getRGBAcolor());
		drawMesh(hitSphere.getMesh());
	}
}

/**
 * @brief Render the hit sphares for each scene road objets. (Debug function)
*/
void Render::showAllRoadHitSpheres() {
	for (size_t i = 0; i < renderableRoads.size(); i++) {
		if (false) showRoadHitSphere(renderableRoads[i]->points, 4.0f, glm::vec3(1, 1, 1));
		if (true) showRoadHitSphere(renderableRoads[i]->shiftedPoints_1, 0.3f, glm::vec3(1, 0, 0));
		if (true) showRoadHitSphere(renderableRoads[i]->shiftedPoints_2, 0.3f, glm::vec3(0, 1, 0));
		if (true) showRoadHitSphere(renderableRoads[i]->trackOne, 0.3f, glm::vec3(0, 0, 1));
		if (true) showRoadHitSphere(renderableRoads[i]->trackTwo, 0.3f, glm::vec3(1, 1, 0));
	}
}

/**
 * @brief Composing the renderable scene from the renderableObjects and the renderableRoads storages.
*/
void Render::renderScrean() {
	if (firstScreen) {
		sceneInit();
		firstScreen = false;
	}

	if (objectsWireframe) wireframeOn();
	for (size_t i = 0; i < renderableObjects.size(); i++) {
		if (!renderableObjects[i].isHidden()) {
			setTexture(renderableObjects[i].getTexture());
			shaderPreDrawingUpdate(renderableObjects[i].getWorldMatrix(), renderableObjects[i].getRGBAcolor());
			drawMesh(renderableObjects[i].getMesh());
			if (hitSphare) showHitSphere(i);
		}
	}
	if (objectsWireframe) wireframeOff();

	if(roadWireframe) wireframeOn();
	for (size_t i = 0; i < renderableRoads.size(); i++) {
		setTexture(objectStorage->getTexture("road.png"));
		shaderPreDrawingUpdate(renderableRoads[i]->getWorldMatrix(), renderableRoads[i]->getRGBAcolor());
		drawVao(renderableRoads[i]->getVAO(), 596);
	}
	if (roadWireframe) wireframeOff();
	if (roadHhitSphare) showAllRoadHitSpheres();
}

/**
 * @brief Remder the next frame. Use the shaders, update the camera and the gui. Render the objects and the curent scene.
*/
void Render::render() {
	fpsCounter::start();
	shader->Use();
	shaderCameraUpdate();

	if (objectStorage->isLoaded()) {
		renderScrean();
	}

	shader->Unuse();
	gui->render();
	rendering();
	setWindowTitle(workingWindow->getWindowTitle() + "  -  " + mapSaver.getLastSave());
	setWindowIcon(objectStorage->getWindowIcon());
	fpsCounter::stop();
}