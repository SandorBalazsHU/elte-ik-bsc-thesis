#include "EventListener.h"
#include "WorkWindow.h"
#include "Render.h"
#include "GUI.h"
#include <glm/gtx/transform2.hpp>

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
	if (mouse.button == SDL_BUTTON_RIGHT) {
		int selectedObjectId = getClickedObjectId(mouse);
		if (selectedObjectId != -1) {
			select(selectedObjectId);
			//std::cout << "HIT" << std::endl;
		}
	}
}

void EventListener::mouseUp(SDL_MouseButtonEvent& mouse) {
	if (mouse.button == SDL_BUTTON_RIGHT) {
		//std::cout << "deselect" << std::endl;
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
	ray_wor = glm::normalize(ray_wor);

	//std::cout << "ray_wor: " << ray_wor.x << "," << ray_wor.y << "," << ray_wor.z << std::endl;

	std::map<float, int> hits;
	for (size_t i = 0; i < render->renderableObjects.size(); i++) {
		glm::vec4 hitSphere = render->renderableObjects[i].getHitSphere();
		glm::vec3 r0 = camera->getCameraPosition();
		glm::vec3 rd = ray_wor;
		glm::vec3 s0 = glm::vec3(hitSphere.x, hitSphere.y, hitSphere.z);
		float sr = hitSphere.w;

		float a = glm::dot(rd, rd);
		glm::vec3 s0_r0 = r0 - s0;
		float b = 2.0 * glm::dot(rd, s0_r0);
		float c = glm::dot(s0_r0, s0_r0) - (sr * sr);
		float disc = b * b - 4.0 * a * c;
		if (disc < 0.0) {
			//std::cout << "NO HIT" << std::endl;
		}
		else {
			hits[glm::distance(s0, camera->getCameraPosition())] = i;
			//std::cout << i << std::endl;
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