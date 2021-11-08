/**
* LRSSG
*/
#include "EventListener.h"
#include "WorkWindow.h"
#include "Render.h"
#include "GUI.h"
#include <glm/gtx/transform2.hpp>
#include <iostream>

EventListener::EventListener(void) {
}

EventListener::~EventListener(void) {
}

void EventListener::bind(WorkWindow* currentWindow) {
	workingWindow = currentWindow;
	camera = workingWindow->getCamera();
	gui = workingWindow->getGUI();
	render = workingWindow->getRender();
}

void EventListener::select(int objectID) {
	Object3D* selectedObject = render->getObject(objectID);
	selectedItems.push_back(selectedObject);
	selectedObject->setOpacity(0.5);
}

void EventListener::deselect() {
	for (size_t i = 0; i < selectedItems.size(); i++) {
		selectedItems[i]->setOpacity(1.0);
	}
	selectedItems.clear();
}

void EventListener::roadDeselect() {
	if(selectedRoads.size() > 0) {
		for (size_t i = 0; i < selectedRoads.size(); i++) {
			selectedRoads[i]->deselect();
		}
		selectedRoads.clear();
	}
}

//TODO Lenyomott F k�zbenti deselect, egyszerre t�bb lenyomott gomb?
void EventListener::keyboardDown(SDL_KeyboardEvent& key) {
	pressedKeys.insert(key.keysym.sym);
	camera->keyboardDown(key);
	if (key.keysym.sym == SDLK_ESCAPE) exit();
	if (key.keysym.sym == SDLK_LCTRL) keepSelect = true;
	if (key.keysym.sym == SDLK_RCTRL) keepSelect = true;
	if (key.keysym.sym == SDLK_f) {
		for (size_t i = 0; i < selectedRoads.size(); i++) {
			selectedRoads[i]->setEndpointLock(false);
		}
	}
}

void EventListener::deleteSelectedItems() {
	if (selectedItems.size() >= 1) {
		for (size_t i = 0; i < selectedItems.size(); i++) {
			render->deleteObject(selectedItems[i]->getRenderID());
		}
	}
}

void EventListener::keyboardUp(SDL_KeyboardEvent& key) {
	pressedKeys.erase(key.keysym.sym);
	camera->keyboardUp(key);
	if (key.keysym.sym == SDLK_LCTRL) keepSelect = false;
	if (key.keysym.sym == SDLK_RCTRL) keepSelect = false;
	if (key.keysym.sym == SDLK_DELETE) deleteSelectedItems();
	if (key.keysym.sym == SDLK_f) {
		for (size_t i = 0; i < selectedRoads.size(); i++) {
			selectedRoads[i]->setEndpointLock(true);
		}
	}
}

//https://en.wikipedia.org/wiki/Rotation_of_axes
void EventListener::mouseMove(SDL_MouseMotionEvent& mouse) {
	camera->mouseMove(mouse);
	if (mouse.state & SDL_BUTTON_RMASK) {

		glm::vec2 shift2D = glm::vec2(mouse.xrel / 4.0f, mouse.yrel / 4.0f);
		float azimuth = camera->getAzimuth();
		glm::mat2 rotateMatrix(glm::cos(azimuth), glm::sin(azimuth), -glm::sin(azimuth), glm::cos(azimuth));
		glm::vec2 rotatedShift2D = rotateMatrix * shift2D;
		glm::vec3 originalShift = glm::vec3(rotatedShift2D.x, 0, rotatedShift2D.y);
		glm::mat3 rotateMatrix2 = glm::rotate(glm::pi<float>()/2.0f, glm::vec3(0, 1, 0));
		glm::vec3 rotatedShift = rotateMatrix2 * originalShift;

		for (size_t i = 0; i < selectedItems.size(); i++) {
			selectedItems[i]->move(rotatedShift);
			if (selectedItems[i]->getDependencyID() > -1) {
				render->updateDynamicObject(selectedItems[i]->getDependencyID());
				//std::cout << selectedItems[i]->getDependencyID() << " , " << selectedItems[i]->getRenderID() << std::endl;
				//std::cout  << std::endl;
			}
		}
	}
}

void EventListener::mouseDown(SDL_MouseButtonEvent& mouse) {
	moseButtonPressed.insert(mouse.button);
	if (mouse.button == SDL_BUTTON_RIGHT) {
		int selectedObjectId = getClickedObjectId(mouse);
		if (selectedObjectId != -1) {
			select(selectedObjectId);
			//std::cout << "HIT" << std::endl;
		}else{
			for (size_t i = 0; i < render->renderableRoads.size(); i++)	{
				if (render->renderableRoads[i]->isClicked(camera->getCameraPosition(), ray)) {
					render->renderableRoads[i]->select();
					selectedRoads.push_back(render->renderableRoads[i]);
					//std::cout << "ROAD HIT" << std::endl;
				}
			}
		}
	}
	if (mouse.button == SDL_BUTTON_LEFT) {
		if(pressedKeys.find(SDLK_f) == pressedKeys.end()) roadDeselect();
	}
}

void EventListener::mouseUp(SDL_MouseButtonEvent& mouse) {
	moseButtonPressed.erase(mouse.button);
	if (mouse.button == SDL_BUTTON_RIGHT) {
		//std::cout << "deselect" << std::endl;
		if(!keepSelect) deselect();
	}
}

void EventListener::mouseWheel(SDL_MouseWheelEvent& wheel) {
	if(moseButtonPressed.find(SDL_BUTTON_RIGHT) == moseButtonPressed.end()) camera->mouseWheel(wheel);
	if (selectedItems.size() > 0) {
		for (size_t i = 0; i < selectedItems.size(); i++) {
			if (wheel.y>0) selectedItems[i]->rotateLeft(10.0f);
			if (wheel.y<0) selectedItems[i]->rotateRight(10.0f);
		}
	}
}

void EventListener::resize(SDL_WindowEvent& window) {
	int with = window.data1;
	int height = window.data2;
	glViewport(0, 0, with, height);
	camera->resize(with, height);
}


void EventListener::exit() {
	workingWindow->close();
}

int EventListener::getClickedObjectId(SDL_MouseButtonEvent& mouse) {
	//Step 0: Get size
	GLint m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);
	float width = m_viewport[2];
	float height = m_viewport[3];

	//Step 1: 3d Normalised Device Coordinates
	float x = (2.0f * mouse.x) / width - 1.0f;
	float y = 1.0f - (2.0f * mouse.y) / height;
	float z = 1.0f;
	glm::vec3 ray_nds = glm::vec3(x, y, z);

	//Step 2: 4d Homogeneous Clip Coordinates
	glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);

	//Step 3: 4d Eye (Camera) Coordinates
	glm::vec4 ray_eye = glm::inverse(camera->getProjectionMatrix()) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

	//Step 4: 4d World Coordinates
	glm::vec3 ray_wor = glm::inverse(camera->getViewMatrix()) * ray_eye;

	//Step 5: Normalize
	ray = glm::normalize(ray_wor);

	//std::cout << "ray_wor: " << ray_wor.x << "," << ray_wor.y << "," << ray_wor.z << std::endl;

	std::map<float, int> hits;
	for (size_t i = 1; i < render->renderableObjects.size(); i++) {
		if (render->renderableObjects[i].isSelectable() && !render->renderableObjects[i].isHidden()) {
			glm::vec4 hitSphere = render->renderableObjects[i].getHitSphere();
			glm::vec3 cameraPosition = camera->getCameraPosition();
			glm::vec3 hitSphereCenter = glm::vec3(hitSphere.x, hitSphere.y, hitSphere.z);
			float hitSphereRadius = hitSphere.w;

			float a = glm::dot(ray, ray);
			glm::vec3 camToPointDirection = cameraPosition - hitSphereCenter;
			float b = 2.0 * glm::dot(ray, camToPointDirection);
			float c = glm::dot(camToPointDirection, camToPointDirection) - (hitSphereRadius * hitSphereRadius);
			float hitDistance = b * b - 4.0 * a * c;
			if (hitDistance < 0.0) {
				//std::cout << "NO HIT" << std::endl;
			}
			else {
				hits[glm::distance(hitSphereCenter, cameraPosition)] = i;
				//std::cout << i << std::endl;
				//ATTENTION !!!!
				//select(i);
			}
		}
	}

	if (!hits.empty()) {
		return hits.begin()->second;
	} else {
		return -1;
	}
}

void EventListener::eventProcessor() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {

		gui->eventHandler(&event);

		if (!gui->isMouseCaptured() && !gui->isKeyboardCaptured()) {
			switch (event.type) {
			case SDL_QUIT:
				exit();
				break;
			case SDL_KEYDOWN:
				keyboardDown(event.key);
				break;
			case SDL_KEYUP:
				keyboardUp(event.key);
				break;
			case SDL_MOUSEBUTTONDOWN:
				mouseDown(event.button);
				break;
			case SDL_MOUSEBUTTONUP:
				mouseUp(event.button);
				break;
			case SDL_MOUSEWHEEL:
				mouseWheel(event.wheel);
				break;
			case SDL_MOUSEMOTION:
				mouseMove(event.motion);
				break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) resize(event.window);
				break;
			}
		}
	}
}