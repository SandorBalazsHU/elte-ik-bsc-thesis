#include "EventListener.h"
#include "WorkWindow.h"
#include "Render.h"
#include "GUI.h"

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

void EventListener::deSelect() {
	for (size_t i = 0; i < selectedItems.size(); i++) {
		selectedItems[i]->setOpacity(1.0);
	}
	selectedItems.clear();
}

void EventListener::keyboardDown(SDL_KeyboardEvent& key) {
	camera->keyboardDown(key);
}

void EventListener::keyboardUp(SDL_KeyboardEvent& key) {
	camera->keyboardUp(key);
}

void EventListener::mouseMove(SDL_MouseMotionEvent& mouse) {
	camera->mouseMove(mouse);
	if (mouse.state & SDL_BUTTON_RMASK) {
		glm::vec3 shift = glm::vec3(mouse.xrel / 4.0f, 0, mouse.yrel / 4.0f);
		for (size_t i = 0; i < selectedItems.size(); i++) {
			selectedItems[i]->move(shift);
		}
	}
}

void EventListener::mouseDown(SDL_MouseButtonEvent& mouse) {

}

void EventListener::mouseUp(SDL_MouseButtonEvent& mouse) {
	if (mouse.button == SDL_BUTTON_RIGHT) {
		std::cout << "Naaaaa????" << std::endl;
		deSelect();
	}
}

void EventListener::mouseWheel(SDL_MouseWheelEvent& wheel) {
	camera->mouseWheel(wheel);
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
				if (event.key.keysym.sym == SDLK_ESCAPE) exit();
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