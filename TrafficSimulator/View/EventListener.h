#pragma once

class WorkWindow;
class GUI;
class Render;

#include<SDL.h>
#include<vector>
#include"Camera.h"
#include"Objects/Object3D.h"
#include"Objects/Object3Droad.h"


class WorkWindow;

class EventListener {
public:
	EventListener();
	~EventListener(void);
	void bind(WorkWindow* currentWindow);
	void eventProcessor();
	void select(int objectID);
	void deselect();
	void roadDeselect();
private:
	WorkWindow* workingWindow;
	Camera* camera;
	GUI* gui;
	Render* render;

	std::vector<Object3D*> selectedItems;
	std::vector<Object3Droad*> selectedRoads;
	bool keepSelect = false;

	glm::vec3 ray;

	bool leftButtonIsPressed = false;
	bool rightButtonIsPressed = false;

	void deleteSelectedItems();

	void keyboardDown(SDL_KeyboardEvent&);
	void keyboardUp(SDL_KeyboardEvent&);
	void mouseMove(SDL_MouseMotionEvent&);
	void mouseDown(SDL_MouseButtonEvent&);
	void mouseUp(SDL_MouseButtonEvent&);
	void mouseWheel(SDL_MouseWheelEvent&);
	void resize(SDL_WindowEvent& window);
	void exit();
	int getClickedObjectId(SDL_MouseButtonEvent& mouse);
};