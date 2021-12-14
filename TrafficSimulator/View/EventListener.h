/**
 * @name Traffic Simulation
 * @file EventListener.h
 * @class EventListener
 * @author Sándor Balázs - AZA6NL
 * @date 2021.12.14.
 * @brief Event listener implementation.
 * Contact: sandorbalazs9402@gmail.com
*/

#pragma once

//Empty classes for avoiding circular includes.
class WorkWindow;
class GUI;
class Render;

#include<SDL.h>
#include<vector>
#include<set>
#include"Camera.h"
#include"Objects/Object3D.h"
#include"Objects/Object3Droad.h"

class EventListener {
public:
	/**
	 * @brief Initialize and bind the event listener.
	 * @param currentWindow The current working window for the event listener.
	*/
	void bind(WorkWindow* currentWindow);

	/**
	 * @brief The main event processor loop.
	*/
	void eventProcessor();

	/**
	 * @brief Object selector.
	 * @param objectID The selected object ID from the mouse ray picker.
	*/
	void select(size_t objectID);

	/**
	 * @brief Object deselect.
	*/
	void deselect();

	/**
	 * @brief Road selector from mouse clicking. (Ray picking for road included.)
	*/
	void roadSelect();

	/**
	 * @brief Road deselect.
	*/
	void roadDeselect();

	/**
	 * @brief Vehicle selector.
	 * @param vehicleID The selected vehicle ID from the mouse ray picker.
	*/
	void vehicleSelect(size_t vehicleID);

	/**
	 * @brief Vehicle deselect.
	*/
	void vehicleDeselect();

	/**
	 * @brief Locking the editor events.
	*/
	void lockEditor();

	/**
	 * @brief Free the editor events.
	*/
	void freeEditor();

	/**
	 * @brief Delete selected items.
	*/
	void deleteSelectedItems();

private:

	/**
	 * @brief The binded pointer to the current working window.
	*/
	WorkWindow* workingWindow;

	/**
	 * @brief The binded pointer to the current camera.
	*/
	Camera* camera;

	/**
	 * @brief The binded pointer to the current GUI.
	*/
	GUI* gui;

	/**
	 * @brief The binded pointer to the current render.
	*/
	Render* render;

	/**
	 * @brief The once pressed key collection.
	*/
	std::set<int> pressedKeys;

	/**
	 * @brief The once pressed mouse buttons collection.
	*/
	std::set<int> moseButtonPressed;

	/**
	 * @brief The list of the selected objects.
	*/
	std::vector<size_t> selectedItems;

	/**
	 * @brief The list of the selected roads.
	*/
	std::vector<size_t> selectedRoads;

	/**
	 * @brief The selected vehicle.
	*/
	size_t selectedVehicle = -1;

	/**
	 * @brief Keep select flag.
	*/
	bool keepSelect = false;

	/**
	 * @brief Editor lock flag.
	*/
	bool editorLock = false;

	/**
	 * @brief The current mouse ray for ray picking calculations.
	*/
	glm::vec3 ray;

	/**
	 * @brief Keyboard down event processor.
	 * @param key The current key event.
	*/
	void keyboardDown(SDL_KeyboardEvent&);

	/**
	 * @brief Keyboard up event processor.
	 * @param key The current key event.
	*/
	void keyboardUp(SDL_KeyboardEvent&);

	/**
	 * @brief The mouse move event processor. (The keyboard move direction is set by camera direction.)
	 * @param mouse The current mouse event.
	*/
	void mouseMove(SDL_MouseMotionEvent&);

	/**
	 * @brief The mouse Down event processor.
	 * @param mouse The current mouse event.
	*/
	void mouseDown(SDL_MouseButtonEvent&);

	/**
	 * @brief The mouse up event processor.
	 * @param mouse The current mouse event.
	*/
	void mouseUp(SDL_MouseButtonEvent&);

	/**
	 * @brief The mouse wheel event processor.
	 * @param mouse The current mouse wheel event.
	*/
	void mouseWheel(SDL_MouseWheelEvent&);

	/**
	 * @brief The resize event processor.
	 * @param mouse The current window event.
	*/
	void resize(SDL_WindowEvent& window);

	/**
	 * @brief Exit event handler.
	*/
	void exit();

	/**
	 * @brief Ray tracing mouse picking implementation.\n
	 * Calculating a ray true the camera and the mouse picked point and test the intersections for all the hit spheres.\n
	 * The vector generation is a revers incremental image synthesis.\n
	 * When have multiple hit sphere in one line we select the one closest to the camera.\n
	 * The function's main step's:\n
	 * Step 0: Get the screen size.\n
	 * Step 1: Get the 3d Normalized mouse coordinates in Device Coordinates.\n
	 * Step 2: Convert the device mouse vector to 4d Homogeneous Clip Coordinates.\n
	 * Step 5: Normalize the world mouse vector.\n
	 * Step 6: Intersect the world mouse vector with all the renderable object's hit spheres.\n
	 * Step 7: Depth test.\n
	 * @param mouse The current mouse click event.
	 * @return The selected item render id.
	*/
	int getClickedObjectId(SDL_MouseButtonEvent& mouse, bool vehicleCheck = false);

	/**
	 * @brief Ray and sphere intersection.
	 * @param ray The ray for the intersection.
	 * @param cameraPosition The camera position for the depth test.
	 * @param hitSphereCenter The sphere center.
	 * @param hitSphereRadius The sphere radius.
	 * @return The sphere and the ray distance.
	*/
	float intersection(glm::vec3 ray, glm::vec3 cameraPosition, glm::vec3 hitSphereCenter, float hitSphereRadius);
};