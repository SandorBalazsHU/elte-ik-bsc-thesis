#pragma once

class WorkWindow;
class GUI;
class Render;

#include<SDL.h>
#include<vector>
#include<set>
#include"Camera.h"
#include"Objects/Object3D.h"
#include"Objects/Object3Droad.h"


class WorkWindow;

class EventListener {
public:
	/*EventListener();
	~EventListener(void);*/
	void bind(WorkWindow* currentWindow);
	void eventProcessor();
	void select(size_t objectID);
	void deselect();
	void roadSelect();
	void roadDeselect();
	void vehicleSelect(size_t vehicleID);
	void vehicleDeselect();
	void lockEditor();
	void freeEditor();
	void deleteSelectedItems();
private:
	WorkWindow* workingWindow;
	Camera* camera;
	GUI* gui;
	Render* render;

	std::set<int> pressedKeys;
	std::set<int> moseButtonPressed;

	std::vector<size_t> selectedItems;
	std::vector<size_t> selectedRoads;
	size_t selectedVehicle = -1;

	bool keepSelect = false;

	bool editorLock = false;

	glm::vec3 ray;

	void keyboardDown(SDL_KeyboardEvent&);
	void keyboardUp(SDL_KeyboardEvent&);
	void mouseMove(SDL_MouseMotionEvent&);
	void mouseDown(SDL_MouseButtonEvent&);
	void mouseUp(SDL_MouseButtonEvent&);
	void mouseWheel(SDL_MouseWheelEvent&);
	void resize(SDL_WindowEvent& window);
	void exit();
	int getClickedObjectId(SDL_MouseButtonEvent& mouse, bool vehicleCheck = false);
	float intersection(glm::vec3 ray, glm::vec3 cameraPosition, glm::vec3 hitSphereCenter, float hitSphereRadius);
};