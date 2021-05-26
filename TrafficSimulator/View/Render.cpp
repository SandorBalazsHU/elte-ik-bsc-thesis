#include "Render.h"
#include "WorkWindow.h"
#include "GUI.h"
#include "fpsCounter.h"

//test
#include "Utilities/ObjParser_OGL3.h"
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
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

void Render::test() {
	loader.load();
	setWindowIcon(loader.getWindowIcon());
	/*glm::vec4 vector(1,2,3,4);
	std::cout << vector.x << vector.y << vector.z << vector.w << std::endl;*/
}

void Render::render() {
	fpsCounter::start();
	faceTestOn();
	clearScrean();
	shader->Use();
	shaderCameraUpdate();

	//ObjectRendering
	setTexture(loader.getTexture("little_car_red_light.png"));
	shaderPreDrawingUpdate(glm::translate(glm::vec3(0, 0, 0)), glm::vec4(1, 1, 1, 1));
	drawMesh(loader.getObject("little_car.obj"));

	setTexture(loader.getTexture("medium_car_yellow_base.png"));
	shaderPreDrawingUpdate(glm::translate(glm::vec3(0, 0, 2)), glm::vec4(1, 1, 1, 1));
	drawMesh(loader.getObject("medium_car.obj"));

	setTexture(loader.getTexture("large_car_blue_base.png"));
	shaderPreDrawingUpdate(glm::translate(glm::vec3(0.3f, 0, 4)), glm::vec4(1, 1, 1, 1));
	drawMesh(loader.getObject("large_car.obj"));

	setTexture(loader.getTexture("bus_yellow_base.png"));
	shaderPreDrawingUpdate(glm::translate(glm::vec3(0.2f, 0, 6)) * glm::rotate(glm::radians(10.0f), glm::vec3(0, 1, 0)), glm::vec4(1, 1, 1, 1));
	drawMesh(loader.getObject("bus.obj"));

	wireframeOn();
	setTexture(loader.getTexture("big_tree_01.png"));
	shaderPreDrawingUpdate(glm::translate(glm::vec3(2, 0, 0)) * glm::scale(glm::vec3(0.1f, 0.1f, 0.1f)), glm::vec4(1, 1, 1, 1));
	drawMesh(loader.getObject("big_tree_01.obj"));
	wireframeOff();

	setTexture(loader.getTexture("big_tree_02.png"));
	shaderPreDrawingUpdate(glm::translate(glm::vec3(-2, 0, 0)) * glm::scale(glm::vec3(0.1f, 0.1f, 0.1f)), glm::vec4(1, 1, 1, 1));
	drawMesh(loader.getObject("big_tree_02.obj"));

	setTexture(loader.getTexture("double_family_house.png"));
	shaderPreDrawingUpdate(glm::translate(glm::vec3(10, 0, 0)) * glm::scale(glm::vec3(0.4f, 0.4f, 0.4f)), glm::vec4(1, 1, 1, 1));
	drawMesh(loader.getObject("double_family_house.obj"));

	setTexture(loader.getTexture("flat_family_house.png"));
	shaderPreDrawingUpdate(glm::translate(glm::vec3(10, 0, 5)) * glm::scale(glm::vec3(0.4f, 0.4f, 0.4f)), glm::vec4(1, 1, 1, 1));
	drawMesh(loader.getObject("flat_family_house.obj"));

	setTexture(loader.getTexture("high_family_house.png"));
	shaderPreDrawingUpdate(glm::translate(glm::vec3(10, 0, 12)) * glm::scale(glm::vec3(0.4f, 0.4f, 0.4f)), glm::vec4(1, 1, 1, 1));
	drawMesh(loader.getObject("high_family_house.obj"));

	setTexture(loader.getTexture("high_family_house_02.png"));
	shaderPreDrawingUpdate(glm::translate(glm::vec3(5, 0, 12)) * glm::scale(glm::vec3(0.4f, 0.4f, 0.4f)), glm::vec4(1, 1, 1, 1));
	drawMesh(loader.getObject("high_family_house_02.obj"));

	setTexture(loader.getTexture("simple_family_house.png"));
	shaderPreDrawingUpdate(glm::translate(glm::vec3(0, 0, 12)) * glm::scale(glm::vec3(0.4f, 0.4f, 0.4f)), glm::vec4(1, 1, 1, 1));
	drawMesh(loader.getObject("simple_family_house.obj"));

	setTexture(loader.getTexture("***"));
	shaderPreDrawingUpdate(glm::translate(glm::vec3(0, 0, 8)) * glm::scale(glm::vec3(0.4f, 0.4f, 0.4f)), glm::vec4(1, 1, 1, 1));
	drawMesh(loader.getObject("***"));

	setTexture(loader.getTexture("desk_round.png"));
	shaderPreDrawingUpdate(glm::translate(glm::vec3(0, -0.1f, 0)) * glm::scale(glm::vec3(3.0f, 3.0f, 3.0f)), glm::vec4(1, 1, 1, 1));
	drawMesh(loader.getObject("desk_round.obj"));

	shader->Unuse();
	gui->render();
	rendering();
	setWindowTitle(workingWindow->getWindowTitle());
	fpsCounter::stop();
}