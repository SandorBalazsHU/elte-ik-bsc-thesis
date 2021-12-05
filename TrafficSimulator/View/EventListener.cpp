/**
* LRSSG
* alarm 1202
* sce to aux
*/

#include "EventListener.h"
#include "WorkWindow.h"
#include "Render.h"
#include "../Model/Graph.h"
#include "GUI.h"
#include <glm/gtx/transform2.hpp>
#include <iostream>

/*EventListener::EventListener(void) {
}

EventListener::~EventListener(void) {
}*/

void EventListener::bind(WorkWindow* currentWindow) {
	workingWindow = currentWindow;
	camera = workingWindow->getCamera();
	gui = workingWindow->getGUI();
	render = workingWindow->getRender();
}

//TODO car selection in eventlistener
void EventListener::select(size_t objectID) {
	if (!editorLock) {
		selectedItems.push_back(objectID);
		render->getObject(objectID)->select();
	}
	if ((render->getObject(objectID)->getName() == "Start sign" || render->getObject(objectID)->getName() == "Stop sign") && editorLock) {
		deselect();
		roadDeselect();
		vehicleDeselect();
		selectedItems.push_back(objectID);
		render->getObject(objectID)->select();
		gui->showEndpointInfo(render->getObject(objectID)->getModelID());
	}
}

void EventListener::deselect() {
	if (editorLock) gui->resetInfoWindow();
	for (size_t i = 0; i < selectedItems.size(); i++) {
		render->getObject(selectedItems[i])->deSelect();
	}
	selectedItems.clear();
}

void EventListener::roadSelect() {
	for (size_t i = 0; i < render->getDynamicObjectsNumber(); i++) {
		if (render->getDynamicObject(i) != NULL) {
			if (render->getDynamicObject(i)->isClicked(camera->getCameraPosition(), ray)) {
				if (editorLock) {
					deselect();
					roadDeselect();
					vehicleDeselect();
				}
				render->getDynamicObject(i)->select();
				selectedRoads.push_back(i);
				if (editorLock) gui->showRoadInfo(render->getDynamicObject(i)->modelID);
			}
		}
	}
}

void EventListener::roadDeselect() {
	if (editorLock) gui->resetInfoWindow();
	if(selectedRoads.size() > 0) {
		for (size_t i = 0; i < selectedRoads.size(); i++) {
			render->getDynamicObject(selectedRoads[i])->deselect();
		}
		selectedRoads.clear();
	}
}

void EventListener::vehicleSelect(size_t vehicleID) {
	render->getVehicle(vehicleID)->select();
	if (editorLock) {
		deselect();
		roadDeselect();
		vehicleDeselect();
		//gui->showVehicleInfo(render->getVehicle(vehicleID)->getModelID());
		gui->showVehicleInfo(vehicleID);
	}
	this->selectedVehicle = vehicleID;
}

void EventListener::vehicleDeselect() {
	if (editorLock) gui->resetInfoWindow();
	if (this->selectedVehicle != -1) {
		render->getVehicle(selectedVehicle)->deSelect();
		//TODO Clear vehicle info in GUI
		this->selectedVehicle = -1;
	}
}

void EventListener::keyboardDown(SDL_KeyboardEvent& key) {
	pressedKeys.insert(key.keysym.sym);
	camera->keyboardDown(key);
	if (key.keysym.sym == SDLK_ESCAPE) exit();
	if (key.keysym.sym == SDLK_LCTRL && !editorLock) keepSelect = true;
	if (key.keysym.sym == SDLK_RCTRL && !editorLock) keepSelect = true;
	if (key.keysym.sym == SDLK_f && !editorLock) {
		for (size_t i = 0; i < selectedRoads.size(); i++) {
			render->getDynamicObject(selectedRoads[i])->setEndpointLock(false);
		}
	}
}

void EventListener::deleteSelectedItems() {
	for (size_t i = 0; i < selectedItems.size(); i++) {
		render->deleteObject(selectedItems[i]);
	}
	selectedItems.clear();
	for (size_t i = 0; i < selectedRoads.size(); i++) {
		render->deleteRoad(selectedRoads[i]);
	}
	selectedRoads.clear();
}

void EventListener::keyboardUp(SDL_KeyboardEvent& key) {
	pressedKeys.erase(key.keysym.sym);
	camera->keyboardUp(key);
	if (key.keysym.sym == SDLK_LCTRL) {
		deselect();
		keepSelect = false;
	}
	if (key.keysym.sym == SDLK_RCTRL) {
		deselect();
		keepSelect = false;
	}
	if (key.keysym.sym == SDLK_DELETE && !editorLock) deleteSelectedItems();


	//-------------------------------------------------------------------------------------------
	if (key.keysym.sym == SDLK_PAGEUP) {
		int shift = -10;
		for (size_t i = 5; i < 17; i++) {
			render->getVehicle(render->addVehicle(i))->move(glm::vec3(shift, 0, shift));
			shift += 3;
		}
	}

	if (key.keysym.sym == SDLK_INSERT) {
		render->getAnimator()->start();
	}
	//---------------------------------------------------------------------------------------

	if (key.keysym.sym == SDLK_f && !editorLock) {
		for (size_t i = 0; i < selectedRoads.size(); i++) {
			render->getDynamicObject(selectedRoads[i])->setEndpointLock(true);
		}
	}
}

//https://en.wikipedia.org/wiki/Rotation_of_axes
void EventListener::mouseMove(SDL_MouseMotionEvent& mouse) {
	camera->mouseMove(mouse);
	if ((mouse.state & SDL_BUTTON_RMASK) && !editorLock) {

		glm::vec2 shift2D = glm::vec2(mouse.xrel / 4.0f, mouse.yrel / 4.0f);
		float azimuth = camera->getAzimuth();
		glm::mat2 rotateMatrix(glm::cos(azimuth), glm::sin(azimuth), -glm::sin(azimuth), glm::cos(azimuth));
		glm::vec2 rotatedShift2D = rotateMatrix * shift2D;
		glm::vec3 originalShift = glm::vec3(rotatedShift2D.x, 0, rotatedShift2D.y);
		glm::mat3 rotateMatrix2 = glm::rotate(glm::pi<float>()/2.0f, glm::vec3(0, 1, 0));
		glm::vec3 rotatedShift = rotateMatrix2 * originalShift;

		for (size_t i = 0; i < selectedItems.size(); i++) {
			
			if (render->getObject(selectedItems[i])->getName() == "Start sign" || render->getObject(selectedItems[i])->getName() == "Stop sign") {
				for (size_t j = 0; j < render->getDynamicObjectsNumber(); j++) {
					if (render->getDynamicObject(j) != NULL) {
						char result = render->getDynamicObject(j)->markerTest(selectedItems[i]);
						if (result == 'A') {
							render->getObject(selectedItems[i])->setPosition(render->getDynamicObject(j)->getEndpointA());
							deselect();
							break;
						}
						if (result == 'B') {
							render->getObject(selectedItems[i])->setPosition(render->getDynamicObject(j)->getEndpointB());
							deselect();
							break;
						}
					}
				}
			}

			render->getObject(selectedItems[i])->move(rotatedShift);

			//std::cout << "SelectedDependency: " << render->getObject(selectedItems[i])->getDependencyID() << std::endl;
			if (render->getObject(selectedItems[i])->getDependencyID() > -1) {
				render->updateDynamicObject(render->getObject(selectedItems[i])->getDependencyID());
			}
		}
	}
}

void EventListener::mouseDown(SDL_MouseButtonEvent& mouse) {
	moseButtonPressed.insert(mouse.button);
	if (mouse.button == SDL_BUTTON_RIGHT) {
		size_t selectedObjectId = getClickedObjectId(mouse);
		if (selectedObjectId != -1) {
			//if (editorLock) deselect();
			select(selectedObjectId);
		}
		size_t selectedVehicleId = getClickedObjectId(mouse, true);
		if (selectedObjectId == -1 && selectedVehicleId != -1 && editorLock) {
			vehicleSelect(selectedVehicleId);
		}
		if (selectedObjectId == -1 && selectedVehicleId == -1) {
			roadSelect();
		}
	}
	if (mouse.button == SDL_BUTTON_LEFT) {
		if(pressedKeys.find(SDLK_f) == pressedKeys.end()) roadDeselect();
		if (editorLock) {
			deselect();
			roadDeselect();
			vehicleDeselect();
		}
	}
}

void EventListener::mouseUp(SDL_MouseButtonEvent& mouse) {
	moseButtonPressed.erase(mouse.button);
	if (mouse.button == SDL_BUTTON_RIGHT && !editorLock) {
		if(!keepSelect) deselect();
	}
}

void EventListener::mouseWheel(SDL_MouseWheelEvent& wheel) {
	if(moseButtonPressed.find(SDL_BUTTON_RIGHT) == moseButtonPressed.end()) camera->mouseWheel(wheel);
	if (selectedItems.size() > 0 && !editorLock) {
		for (size_t i = 0; i < selectedItems.size(); i++) {
			if (wheel.y>0) render->getObject(selectedItems[i])->rotateLeft(10.0f);
			if (wheel.y<0) render->getObject(selectedItems[i])->rotateRight(10.0f);
		}
	}
}

void EventListener::resize(SDL_WindowEvent& window) {
	int with = window.data1;
	int height = window.data2;
	glViewport(0, 0, with, height);
	camera->resize(with, height);
}

void EventListener::lockEditor() {
	this->editorLock = true;
}

void EventListener::freeEditor() {
	this->editorLock = false;
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