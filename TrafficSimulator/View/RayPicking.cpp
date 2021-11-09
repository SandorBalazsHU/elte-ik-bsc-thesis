#include "EventListener.h"
#include "Render.h"
#include <map>

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
			} else {
				hits[glm::distance(hitSphereCenter, cameraPosition)] = i;
				//std::cout << i << std::endl;
			}
		}
	}

	if (!hits.empty()) {
		return hits.begin()->second;
	}
	else {
		return -1;
	}
}