#include "Render.h"

Render::Render(void) {
}

Render::~Render(void) {
}

void Render::bind(WorkWindow* currentWindow) {
	window = currentWindow;
	camera = window->getCamera();
	shader = window->getShader();
}

void Render::faceTestOn() {
	glEnable(GL_CULL_FACE);
}

void Render::faceTestOff() {
	glDisable(GL_CULL_FACE);
}

void Render::shaderUpdate() {
	shader->SetUniform("eye_pos", camera->getCameraPosition());
}

void Render::render() {

}