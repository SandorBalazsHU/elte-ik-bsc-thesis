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

//TODO Lenyomott F közbenti deselect, egyszerre több lenyomott gomb?
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
			
			if (selectedItems[i]->getName() == "Start sign" || selectedItems[i]->getName() == "Stop sign") {
				bool result = false;
				for (size_t j = 0; j < render->renderableRoads.size(); j++) {
					result = render->renderableRoads[j]->markerTest(selectedItems[i]);
					//if(result) selectedItems[i]->setPosition(render->renderableRoads[j]->getEndpointB());
				}
				/*if (result && (pressedKeys.find(SDLK_f) == pressedKeys.end())) {
					deselect();
					break;
				}*/
			}

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