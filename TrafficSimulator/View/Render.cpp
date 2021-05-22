#include "Render.h"
#include "WorkWindow.h"
#include "GUI.h"
#include "fpsCounter.h"

//test
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
	test();
	fpsCounter::bind();
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

void Render::test() {
	loader.load();
	setWindowIcon(loader.getWindowIcon());
}

void Render::render() {
	fpsCounter::start();
	faceTestOn();
	clearScrean();
	shader->Use();
	shaderCameraUpdate();

	//ObjectRendering
	setTexture(loader.textures["little_car_red_light.png"]);
	shaderPreDrawingUpdate(glm::translate(glm::vec3(0, 0, 0)), glm::vec4(1, 1, 1, 1));
	drawMesh(loader.objects["little_car.obj"]);

	shader->Unuse();
	gui->render();
	rendering();
	setWindowTitle(workingWindow->getWindowTitle());
	fpsCounter::stop();
}