#include "Render.h"
#include "WorkWindow.h"
#include <SDL.h>
#include <iostream>
#include <sstream>

Render::Render(void) {
}

Render::~Render(void) {
}

void Render::bind(WorkWindow* currentWindow) {
	workingWindow = currentWindow;
	camera = workingWindow->getCamera();
	shader = workingWindow->getShader();
	window = workingWindow->getWindow();
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

void Render::setTexture(Texture2D texture) {
	shader->SetTexture("currentTexture", 0, texture);
}

void Render::shaderCameraUpdate() {
	shader->SetUniform("cameraPosition", camera->getCameraPosition());
}

void Render::shaderPreDrawingUpdate(glm::mat4 worldMatrix, glm::mat4 color) {
	shader->SetUniform("worldMatrix", worldMatrix);
	shader->SetUniform("worldInverseTransposeMatrix", glm::transpose(glm::inverse(worldMatrix)));
	shader->SetUniform("projectionViewWorldMatrix", camera->getProjectionViewMatrix() * worldMatrix);
	shader->SetUniform("diffuseMaterial", color);
}

void Render::drawMesh(std::unique_ptr<Mesh> mesh) {
	mesh->draw();
}

void Render::drawVao(VertexArrayObject& vao, int trianglesNumber) {
	vao.Bind();
	glDrawElements(GL_TRIANGLES, trianglesNumber, GL_UNSIGNED_INT, 0);
	vao.Unbind();
}

void Render::render() {
	faceTestOn();
	clearScrean();
	shader->Use();
	shaderCameraUpdate();

	//ObjectRendering

	shader->Unuse();
	SDL_GL_SwapWindow(window);
	std::stringstream window_title;
	window_title << "TEST";
	SDL_SetWindowTitle(window, window_title.str().c_str());
}