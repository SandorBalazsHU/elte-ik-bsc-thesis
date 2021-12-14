/**
 * @name Traffic Simulation
 * @file Render.h
 * @class Render
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief The 3D Scene render class
 * Contact: sandorbalazs9402@gmail.com
*/

// AX400

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
	animator.bind(this);
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
 * @brief Render mode configurator - Wire frame object drawing on.
*/
void  Render::wireframeOn() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

/**
* @brief Render mode configurator - Wire frame object drawing off.
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
 * @brief Camera position updating in the shader variable.
*/
void Render::shaderCameraUpdate() {
	shader->SetUniform("cameraPosition", camera->getCameraPosition());
}

/**
 * @brief Update the light position in the shader.
*/
void Render::shaderLightUpdate() {
	if (lightCameraTracking) {
		shader->SetUniform("lightPosition", camera->getCameraPosition()); 
	} else {
		shader->SetUniform("lightPosition", lightPosition);
	}

	shader->SetUniform("ambientLight",	   this->ambientLight);
	shader->SetUniform("diffuseLight",	   this->diffuseLight);
	shader->SetUniform("specularLight",	   this->specularLight);

	shader->SetUniform("ambientMaterial",  this->ambientMaterial);
	shader->SetUniform("diffuseMaterial",  this->diffuseMaterial);
	shader->SetUniform("specularMaterial", this->specularMaterial);

	shader->SetUniform("specularPower", this->specularPower);
}

/**
 * @brief Window title text setter.
 * @param title The new window title.
*/
void Render::setWindowTitle(std::string title) {
	std::stringstream window_title;
	window_title << title << "  -  " << fpsCounter::getAverageFPS() << " fps";
	SDL_SetWindowTitle(window, window_title.str().c_str());
}

/**
 * @brief Getter for Animator.
*/
Animator* Render::getAnimator() {
	return &animator;
}

/**
 * @brief Getter for the working window pointer.
 * @return Pointer for the current working window.
*/
WorkWindow* Render::getWorkingWindow() {
	return workingWindow;
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
 * @brief Getter for the GUI.
 * @return The current GUI.
*/
GUI* Render::getGui() {
	return gui;
}

/**
 * @brief Upload the shader data before rendering an object.
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
 * @brief Drawing a dynamically generated object.
 * @param VAO The dynamically generated object for drawing.
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
 * @brief Initialize the scene. Render the first objects.
*/
void Render::sceneInit() {
	std::vector<int>& firstSceneElements = objectStorage->getFirstSceneElements();
	for (size_t i = 0; i < firstSceneElements.size(); i++) {
		size_t objectID = addObject(firstSceneElements[i]);
		getObject(objectID)->setSelectable(false);
	}
}

/**
 * @brief Clean and renew the map editor.
*/
void Render::clear() {
	mapSaver.reset();
	renderableObjects.clear();
	renderableRoads.clear();
	renderableVehicles.clear();
	animator.clear();
	sceneInit();
}

/**
 * @brief Add new renderable object from the preloaded object storage to the scene.
 * @param id New object storage ID.
 * @return The new object render ID.
*/
int Render::addObject(int id) {
	size_t newRenderID = renderableObjects.size();
	renderableObjects.push_back(objectStorage->getObject3D(id, newRenderID));
	return newRenderID;
}

/**
 * @brief Delete a scene object by renderID.
 * \attention This cause pointer invalidation in the vector. Use the updateRenderIDs and rebindRoads for repairing pointers.
 * @param renderID renderableObjects for the 
*/
void Render::deleteObject(int renderID) {
	if (!renderableObjects[renderID].isProtected()) {
		renderableObjects[renderID].erase();
	}
}

/**
 * @brief Delete a scene object by renderID.
 * /attention This cause pointer invalidation in the vector. Use the updateRenderIDs and rebindRoads for repairing pointers.
 * @param renderID renderID
*/
void Render::deleteRoad(int dynamicRenderID) {

	for (size_t i = 0; i < 4; i++) renderableObjects[renderableRoads[dynamicRenderID]->getTrackBalls()[i]].erase();
	for (size_t i = 0; i < 2; i++) renderableObjects[renderableRoads[dynamicRenderID]->getRoadEndCircles()[i]].erase();

	delete renderableRoads[dynamicRenderID];
	renderableRoads[dynamicRenderID] = NULL;
}

/**
 * @brief Add dynamically generated road to the scene.
 * /attention The road objects have 4 marker and 2 filler object for dependencies.
 * @return The new road dynamicRenderID.
*/
int Render::addRoad() {
	Object3Droad* road = new Object3Droad();
	renderableRoads.push_back(road);

	for (size_t i = 0; i < 2; i++) addObject(2);
	for (size_t i = 0; i < 4; i++) addObject(1);

	size_t objectsNumber = getObjectsNumber();
	size_t dynamicObjectsNumber = getDynamicObjectsNumber();

	road->bind(this, dynamicObjectsNumber - 1, objectsNumber - 1, objectsNumber - 2, objectsNumber-3, objectsNumber - 4, objectsNumber - 5, objectsNumber - 6);
	
	return dynamicObjectsNumber - 1;
}

/**
 * @brief Add a new vehicle to the scene, from the objectStorage.
 * @param objectStorageID The addable vehicle object storage ID.
 * @return The added vehicle renderID.
*/
size_t Render::addVehicle(int objectStorageID, size_t modelID) {
	size_t newRenderID = renderableVehicles.size();
	renderableVehicles.push_back(objectStorage->getObject3Dvehicle(objectStorageID, newRenderID));
	renderableVehicles[newRenderID].setModelID(modelID);
	return newRenderID;
}

/**
 * @brief Delete a vehicle by renderID.
 * @param renderableVehicleID The deletable vehicle renderID.
*/
void Render::deleteVehicle(size_t renderableVehicleID) {
	if (!renderableVehicles[renderableVehicleID].isProtected()) {
		renderableVehicles[renderableVehicleID].erase();
	}
}

/**
 * @brief Get scene preloaded object from the scene render storage.
 * @param id renderID
 * @return The scene object.
*/
Object3D* Render::getObject(size_t id) {
	return &renderableObjects[id];
}

/**
 * @brief Getter for the RenderableRoads size.
 * @return The renderableRoads size.
*/
size_t Render::getObjectsNumber() {
	return renderableObjects.size();
}

/**
 * @brief Getter for the RenderableRoads.
 * @param renderID The needed object dynamicRenderID.
 * @return The needed object.
*/
Object3Droad* Render::getDynamicObject(size_t dynamicRenderID) {
	return renderableRoads[dynamicRenderID];
}

/**
 * @brief Getter for the renderableRoads size.
 * @return The renderableRoads size.
*/
size_t Render::getDynamicObjectsNumber() {
	return renderableRoads.size();
}

/**
 * @brief Getter for the renderableVehicles;
 * @param renderableVehicleID The needed vehicle's id.
 * @return The needed vehicle.
*/
Object3Dvehicle* Render::getVehicle(size_t renderableVehicleID) {
	return &renderableVehicles[renderableVehicleID];
}

/**
 * @brief Getter for the renderableVehicles size.
 * @return The renderableVehicles size.
*/
size_t Render::getVehiclesNumber() {
	return this->renderableVehicles.size();
}

/**
 * @brief Update a dynamically generated object status by dynamicRenderID.
 * @param The updatable dynamicRenderID
*/
void Render::updateDynamicObject(size_t dynamicRenderID) {
	renderableRoads[dynamicRenderID]->update();
	for (size_t i = 0; i < renderableRoads.size(); i++) {
		if (i != dynamicRenderID && renderableRoads[dynamicRenderID] != NULL && renderableRoads[i] != NULL) {
			renderableRoads[dynamicRenderID]->stuckTest(i);
		}
	}
	for (size_t i = 0; i < renderableObjects.size(); i++) {
		if (!renderableObjects[i].isDeleted()) {
			if (renderableObjects[i].getName() == "Start sign" || renderableObjects[i].getName() == "Stop sign") {
				renderableRoads[dynamicRenderID]->markerTest(i);
			}
		}
	}
}

/**
 * @brief Render the hit spheres for each scene objects. (Debug function)
*/
void Render::showHitSphere(size_t renderID) {
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
 * @brief Render the hit spheres for each scene vehicle objects. (Debug function)
*/
void  Render::showVehicleHitSphere(size_t vehicleRenderID) {
	Object3D hitSphere = objectStorage->getObject3D(1);
	hitSphere.setPosition(glm::vec3(renderableVehicles[vehicleRenderID].getHitSphere().x,
		renderableVehicles[vehicleRenderID].getHitSphere().y,
		renderableVehicles[vehicleRenderID].getHitSphere().z));
	hitSphere.setScale(glm::vec3(renderableVehicles[vehicleRenderID].getHitSphere().w,
		renderableVehicles[vehicleRenderID].getHitSphere().w,
		renderableVehicles[vehicleRenderID].getHitSphere().w));
	hitSphere.setOpacity(0.2f);

	setTexture(hitSphere.getTexture());
	shaderPreDrawingUpdate(hitSphere.getWorldMatrix(), hitSphere.getRGBAcolor());
	drawMesh(hitSphere.getMesh());
}

/**
 * @brief Hide the desk for object photo mode. Function on.(Debug function)
*/
void Render::photoModeOn() {
	getObject(0)->hide();
}

/**
 * @brief Hide the desk for object photo mode. Function off. (Debug function)
*/
void Render::photoModeOff() {
	getObject(0)->show();
}

/**
 * @brief Render the hit spheres for each scene road objects. (Debug function)
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
 * @brief Render the hit spheres for each scene road objects. (Debug function)
*/
void Render::showAllRoadHitSpheres() {
	for (size_t i = 0; i < renderableRoads.size(); i++) {
		if (renderableRoads[i] != NULL) {
			if (showRoadHitSphereMiddle) showRoadHitSphere(renderableRoads[i]->points, 4.0f, glm::vec3(1, 1, 1));
			if (showRoadSide01) showRoadHitSphere(renderableRoads[i]->shiftedpoints_1, 0.3f, glm::vec3(1, 0, 0));
			if (showRoadSide02) showRoadHitSphere(renderableRoads[i]->shiftedpoints_2, 0.3f, glm::vec3(0, 1, 0));
			if (showRoadTrack01) showRoadHitSphere(renderableRoads[i]->trackOne, 0.3f, glm::vec3(0, 0, 1));
			if (showRoadTrack02) showRoadHitSphere(renderableRoads[i]->trackTwo, 0.3f, glm::vec3(1, 1, 0));
		}
	}
}

/**
 * @brief Lock the editor functions
*/
void Render::lockEditor() {
	this->editorLock = true;
	this->workingWindow->getEventListener()->lockEditor();
	for (size_t i = 0; i < this->getDynamicObjectsNumber(); i++) {
		if (this->getDynamicObject(i) != NULL) {
			this->getDynamicObject(i)->lockEditor();
		}
	}
}

/**
 * @brief Free the editor functions.
*/
void Render::freeEditor() {
	this->editorLock = false;
	this->workingWindow->getEventListener()->freeEditor();
	for (size_t i = 0; i < this->getDynamicObjectsNumber(); i++) {
		if (this->getDynamicObject(i) != NULL) {
			this->getDynamicObject(i)->freeEditor();
		}
	}
}

/**
 * @brief Return true if the editor locked.
*/
bool Render::isEditorLoced() {
	return this->editorLock;
}

/**
 * @brief Delete all active vehicles.
*/
void Render::clearVehicles() {
	this->renderableVehicles.clear();
}

/**
 * @brief Composing the renderable scene from the renderableObjects and the renderableRoads storages.
*/
void Render::renderScrean() {
	if (firstScreen) {
		sceneInit();
		firstScreen = false;
	}

	this->animator.update();

	if (objectsWireframe) wireframeOn();
	for (size_t i = 0; i < renderableObjects.size(); i++) {
		if (!renderableObjects[i].isDeleted()) {
			if (!renderableObjects[i].isHidden()) {
				setTexture(renderableObjects[i].getTexture());
				shaderPreDrawingUpdate(renderableObjects[i].getWorldMatrix(), renderableObjects[i].getRGBAcolor());
				drawMesh(renderableObjects[i].getMesh());
				if (hitSphare) showHitSphere(i);
			}
		}
	}

	for (size_t i = 0; i < renderableVehicles.size(); i++) {
		if (!renderableVehicles[i].isDeleted()) {
			if (!renderableVehicles[i].isHidden()) {
				setTexture(renderableVehicles[i].getTexture());
				shaderPreDrawingUpdate(renderableVehicles[i].getWorldMatrix(), renderableVehicles[i].getRGBAcolor());
				drawMesh(renderableVehicles[i].getMesh());
				if (vehicleHitSphare) showVehicleHitSphere(i);
			}
		}
	}
	if (objectsWireframe) wireframeOff();

	if(roadWireframe) wireframeOn();
	for (size_t i = 0; i < renderableRoads.size(); i++) {
		if (renderableRoads[i] != NULL) {
			setTexture(objectStorage->getTexture("road.png"));
			shaderPreDrawingUpdate(renderableRoads[i]->getWorldMatrix(), renderableRoads[i]->getRGBAcolor());
			drawVao(renderableRoads[i]->getVAO(), 596);
		}
	}
	if (roadWireframe) wireframeOff();
	if (roadHhitSphare) showAllRoadHitSpheres();
}

/**
 * @brief Render the next frame. Use the shader, update the camera and the GUI. Render the objects and the current scene.
*/
void Render::render() {
	fpsCounter::start();
	shader->Use();
	shaderCameraUpdate();
	shaderLightUpdate();

	if (objectStorage->isLoaded()) {
		renderScrean();
	}

	shader->Unuse();
	gui->render();
	rendering();
	setWindowTitle(workingWindow->getWindowTitle() + "  -  Map name: " + mapSaver.getLastSave());
	setWindowIcon(objectStorage->getWindowIcon());
	fpsCounter::stop();
}