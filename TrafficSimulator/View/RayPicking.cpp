/**
 * @name Traffic Simulation
 * @file RayPicking.cpp
 * @class EventListener
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.09.
 * @brief OpenGL 3D system initialization and window open.
 * Contact: sandorbalazs9402@gmail.com
*/

#include "EventListener.h"
#include "Render.h"
#include <map>

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
 * @return The selected item renderid.
*/
int EventListener::getClickedObjectId(SDL_MouseButtonEvent& mouse) {
	//Step 0: Get the screen size.
	GLint m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);
	float width = m_viewport[2];
	float height = m_viewport[3];

	//Step 1: Get the 3d Normalized mouse coordinates in Device Coordinates.
	float x = (2.0f * mouse.x) / width - 1.0f;
	float y = 1.0f - (2.0f * mouse.y) / height;
	float z = 1.0f;
	glm::vec3 ray_nds = glm::vec3(x, y, z);

	//Step 2: Convert the device mouse vector to 4d Homogeneous Clip Coordinates.
	glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);

	//Step 3: Convert the clip mouse vector to 4d Eye Camera Coordinates.
	glm::vec4 ray_eye = glm::inverse(camera->getProjectionMatrix()) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

	//Step 4: Convert the camera mouse vector to 4d World Coordinates.
	glm::vec3 ray_world = glm::inverse(camera->getViewMatrix()) * ray_eye;

	//Step 5: Normalize the world mouse vector.
	ray = glm::normalize(ray_world);

	//Set of the found hits for the distance checking.
	std::map<float, int> hits;

	//Step 6: Intersect the world mouse vector with all the renderable object's hit spheres.
	for (size_t i = 1; i < render->renderableObjects.size(); i++) {
		//Don't check when the object not selectable or hidden.
		if (render->renderableObjects[i].isSelectable() && !render->renderableObjects[i].isHidden()) {
			//Parameter preparation.
			glm::vec4 hitSphere = render->renderableObjects[i].getHitSphere();
			glm::vec3 cameraPosition = camera->getCameraPosition();
			glm::vec3 hitSphereCenter = glm::vec3(hitSphere.x, hitSphere.y, hitSphere.z);
			float hitSphereRadius = hitSphere.w;

			//Ray and hit sphere center distance calculation.
			float a = glm::dot(ray, ray);
			glm::vec3 camToPointDirection = cameraPosition - hitSphereCenter;
			float b = 2.0f * glm::dot(ray, camToPointDirection);
			float c = glm::dot(camToPointDirection, camToPointDirection) - (hitSphereRadius * hitSphereRadius);
			float hitDistance = b * b - 4.0f * a * c;
			if (hitDistance >= 0.0f) {
				//Set by the camera and the object distance for the depth test.
				hits[glm::distance(hitSphereCenter, cameraPosition)] = i;
			}
		}
	}

	//Step 7: Depth test.
	if (!hits.empty()) {
		return hits.begin()->second;
	} else {
		return -1;
	}
}