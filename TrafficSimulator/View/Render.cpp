/**
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
	//faceTestOn();
	multisamplingOn();
	vsyncOn();
	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	//wireframeOn();
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
	renderableObjects.push_back(objectStorage->getObject3D(id, renderableObjects.size()));
	return renderableObjects.size()-1;
}

void Render::deleteObject(int renderID) {
	if (!renderableObjects[renderID].isProtected()) {
		renderableObjects.erase(renderableObjects.begin() + renderID);
		updateRenderIDs();
		rebindRoads();
	}
}

void Render::updateRenderIDs() {
	for (size_t i = 0; i < renderableObjects.size(); i++) {
		renderableObjects[i].setRenderID(i);
	}
}

//Végignézzük az összes dependenciával rendelkezõ objektumot és újrakötjük.
//Repair pointers corrupted by deleting  .
//TODO Mi van ha nem négy van?
void Render::rebindRoads() {
	if (renderableRoads.size() > 0) {
		std::vector<Object3D*> rebindableObjects;
		for (size_t i = 0; i < renderableObjects.size(); i++) {
			if (renderableObjects[i].getDependencyID() > -1) {
				rebindableObjects.push_back(&renderableObjects[i]);
				//std::cout << renderableObjects[i].getDependencyID() << std::endl;
			}
		}
		//std::cout << std::endl << std::endl << rebindableObjects.size() << std::endl << std::endl;
		for (size_t i = 0; i < rebindableObjects.size(); i += 6) {
			//std::cout << i << std::endl;
			renderableRoads[rebindableObjects[i]->getDependencyID()]->reBind(rebindableObjects[i], rebindableObjects[i + 1], rebindableObjects[i + 2], rebindableObjects[i + 3], rebindableObjects[i + 4], rebindableObjects[i + 5]);
		}
	}
}

int Render::addRoad() {
	Object3Droad* road = new Object3Droad();
	addObject(1);
	addObject(1);
	addObject(1);
	addObject(1);
	addObject(23);
	addObject(23);

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

Object3D* Render::getObject(int id) {
	return &renderableObjects[id];
}

void Render::updateDynamicObject(int id) {
	renderableRoads[id]->update();
}

void Render::renderScrean() {
	if (firstScreen) {
		sceneInit();
		firstScreen = false;
	}

	for (size_t i = 0; i < renderableObjects.size(); i++) {
		if (!renderableObjects[i].isHidden()) {
			setTexture(renderableObjects[i].getTexture());
			shaderPreDrawingUpdate(renderableObjects[i].getWorldMatrix(), renderableObjects[i].getRGBAcolor());
			drawMesh(renderableObjects[i].getMesh());
		}

		//Road Line Debug
		if (false) {
			if (renderableRoads.size() >= 1) {
				for (size_t i = 0; i < renderableRoads[0]->points.size(); i += 1) {
					Object3D hitSphere = objectStorage->getObject3D(1);
					hitSphere.setPosition(renderableRoads[0]->points[i]);
					hitSphere.setScale(glm::vec3(0.3f, 0.3f, 0.3f));
					hitSphere.setRGBcolor(glm::vec3(1, 0, 0));
					hitSphere.setOpacity(0.2f);
					setTexture(hitSphere.getTexture());
					shaderPreDrawingUpdate(hitSphere.getWorldMatrix(), hitSphere.getRGBAcolor());
					drawMesh(hitSphere.getMesh());
				}
			}
		}

		if (false) {
			if (renderableRoads.size() >= 1) {
				for (size_t i = 0; i < renderableRoads[0]->shiftedPoints_1.size(); i += 1) {
					Object3D hitSphere = objectStorage->getObject3D(1);
					hitSphere.setPosition(renderableRoads[0]->shiftedPoints_1[i]);
					hitSphere.setScale(glm::vec3(0.3f, 0.3f, 0.3f));
					hitSphere.setRGBcolor(glm::vec3(1, 0, 0));
					hitSphere.setOpacity(0.2f);
					setTexture(hitSphere.getTexture());
					shaderPreDrawingUpdate(hitSphere.getWorldMatrix(), hitSphere.getRGBAcolor());
					drawMesh(hitSphere.getMesh());
				}
			}
		}

		if (false) {
			if (renderableRoads.size() >= 1) {
				for (size_t i = 0; i < renderableRoads[0]->shiftedPoints_2.size(); i += 1) {
					Object3D hitSphere = objectStorage->getObject3D(1);
					hitSphere.setPosition(renderableRoads[0]->shiftedPoints_2[i]);
					hitSphere.setScale(glm::vec3(0.3f, 0.3f, 0.3f));
					hitSphere.setRGBcolor(glm::vec3(1, 0, 0));
					hitSphere.setOpacity(0.2f);
					setTexture(hitSphere.getTexture());
					shaderPreDrawingUpdate(hitSphere.getWorldMatrix(), hitSphere.getRGBAcolor());
					drawMesh(hitSphere.getMesh());
				}
			}
		}

		//Hit sphare debug
		if (false) {
			Object3D hitSphere = objectStorage->getObject3D(1);
			hitSphere.setPosition(glm::vec3(renderableObjects[i].getHitSphere().x,
				renderableObjects[i].getHitSphere().y,
				renderableObjects[i].getHitSphere().z));
			hitSphere.setScale(glm::vec3(renderableObjects[i].getHitSphere().w,
				renderableObjects[i].getHitSphere().w,
				renderableObjects[i].getHitSphere().w));
			hitSphere.setOpacity(0.2f);

			setTexture(hitSphere.getTexture());
			shaderPreDrawingUpdate(hitSphere.getWorldMatrix(), hitSphere.getRGBAcolor());
			drawMesh(hitSphere.getMesh());
		}
	}
	//wireframeOn();
	for (size_t i = 0; i < renderableRoads.size(); i++) {
		setTexture(objectStorage->getTexture("road.png"));
		shaderPreDrawingUpdate(renderableRoads[i]->getWorldMatrix(), renderableRoads[i]->getRGBAcolor());
		drawVao(renderableRoads[i]->getVAO(), 596);
	}
	//wireframeOff();

}

void Render::render() {
	fpsCounter::start();
	//faceTestOn();
	//clearScrean();
	shader->Use();
	shaderCameraUpdate();

	if (objectStorage->isLoaded()) {
		renderScrean();
	}

	shader->Unuse();
	gui->render();
	rendering();
	setWindowTitle(workingWindow->getWindowTitle());
	//TODO: NOT A GOD SOLUTION
	setWindowIcon(objectStorage->getWindowIcon());
	fpsCounter::stop();
}