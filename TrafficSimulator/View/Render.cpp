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

Render::Render(void) {
}

Render::~Render(void) {
}

void Render::bind(WorkWindow* currentWindow) {
	workingWindow = currentWindow;
	camera = workingWindow->getCamera();
	shader = workingWindow->getShader();
	window = workingWindow->getWindow();
	gui = workingWindow->getGUI();
	objectStorage = workingWindow->getObjectStorage();
	setWindowIcon(objectStorage->getWindowIcon());
	fpsCounter::bind();
	/*void vsyncOff();
	fpsCounter::fpsLimiterOn();
	fpsCounter::setFpsLimit(60);*/
}

void Render::multisamplingOn() {
	glEnable(GL_MULTISAMPLE);
}

void Render::multisamplingOff() {
	glDisable(GL_MULTISAMPLE);
}

void Render::vsyncOn() {
	SDL_GL_SetSwapInterval(1);
}

void Render::vsyncOff() {
	SDL_GL_SetSwapInterval(0);
}

void  Render::wireframeOn() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void  Render::wireframeOff() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Render::faceTestOn() {
	glEnable(GL_CULL_FACE);
}

void Render::faceTestOff() {
	glDisable(GL_CULL_FACE);
}

void Render::clearScrean() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render::setTexture(Texture2D& texture) {
	shader->SetTexture("currentTexture", 0, texture);
}

void Render::shaderCameraUpdate() {
	shader->SetUniform("cameraPosition", camera->getCameraPosition());
}

void Render::setWindowTitle(std::string title) {
	std::stringstream window_title;
	window_title << title << " - " << fpsCounter::getAverageFPS();
	SDL_SetWindowTitle(window, window_title.str().c_str());
}

void Render::shaderPreDrawingUpdate(glm::mat4 worldMatrix, glm::vec4 color) {
	shader->SetUniform("worldMatrix", worldMatrix);
	shader->SetUniform("worldInverseTransposeMatrix", glm::transpose(glm::inverse(worldMatrix)));
	shader->SetUniform("projectionViewWorldMatrix", camera->getProjectionViewMatrix() * worldMatrix);
	shader->SetUniform("diffuseMaterial", color);
}

void Render::drawMesh(std::unique_ptr<Mesh>& mesh) {
	mesh->draw();
}

void Render::drawVao(VertexArrayObject& vao, int trianglesNumber) {
	vao.Bind();
	glDrawElements(GL_TRIANGLES, trianglesNumber, GL_UNSIGNED_INT, 0);
	vao.Unbind();
}

void Render::setWindowIcon(SDL_Surface* windowIcon) {
	SDL_SetWindowIcon(window, windowIcon);
}

void Render::rendering() {
	SDL_GL_SwapWindow(window);
}

void Render::sceneInit() {
	std::vector<int>& firstSceneElements = objectStorage->getFirstSceneElements();
	for (size_t i = 0; i < firstSceneElements.size(); i++) {
		addObject(firstSceneElements[i]);
	}
}

int Render::addObject(int id) {
	renderableObjects.push_back(objectStorage->getObject3D(id));
	return renderableObjects.size()-1;
}

Object3D* Render::getObject(int id) {
	return &renderableObjects[id];
}

void Render::renderScrean() {
	Object3Droad road;
	if (firstScreen) {
		sceneInit();
		firstScreen = false;
	}
	road.generate();
	setTexture(objectStorage->getTexture("road.png"));
	shaderPreDrawingUpdate(glm::translate(glm::vec3(0, 0, 0)), glm::vec4(1, 1, 1, 1));
	drawVao(road.getVAO(), 1000);

	for (size_t i = 0; i < renderableObjects.size(); i++) {
		setTexture(renderableObjects[i].getTexture());
		shaderPreDrawingUpdate(renderableObjects[i].getWorldMatrix(), renderableObjects[i].getRGBAcolor());
		drawMesh(renderableObjects[i].getMesh());

		/*//debug
		Object3D hitSphere = objectStorage->getObject3D(1);
		hitSphere.setPosition(glm::vec3(renderableObjects[i].getHitSphere().x,
			renderableObjects[i].getHitSphere().y, 
			renderableObjects[i].getHitSphere().z));
		hitSphere.setScale(glm::vec3(renderableObjects[i].getHitSphere().w,
			renderableObjects[i].getHitSphere().w,
			renderableObjects[i].getHitSphere().w));
		hitSphere.setOpacity(0.5f);

		setTexture(hitSphere.getTexture());
		shaderPreDrawingUpdate(hitSphere.getWorldMatrix(), hitSphere.getRGBAcolor());
		drawMesh(hitSphere.getMesh());*/
	}
}

void Render::render() {
	fpsCounter::start();
	faceTestOn();
	clearScrean();
	shader->Use();
	shaderCameraUpdate();

	if (objectStorage->isLoaded()) {
		renderScrean();
	}

	shader->Unuse();
	gui->render();
	rendering();
	setWindowTitle(workingWindow->getWindowTitle());
	fpsCounter::stop();
}