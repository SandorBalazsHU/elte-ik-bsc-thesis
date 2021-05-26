/*#include "RayPicking.h"

void Simulation::MouseDown(SDL_MouseButtonEvent& mouse) {
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
	glm::vec4 ray_eye = glm::inverse(camera.GetProj()) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

	//Step 4: 4d World Coordinates
	glm::vec3 ray_wor = glm::inverse(camera.GetViewMatrix()) * ray_eye;

	//Step 5: Normalize
	ray_wor = glm::normalize(ray_wor);

	std::cout << "ray_wor: " << ray_wor.x << "," << ray_wor.y << "," << ray_wor.z << std::endl;

	std::map<float, int> hits;
	for (size_t i = 0; i < 4; i++) {
		glm::vec3 r0 = camera.GetEye();
		glm::vec3 rd = ray_wor;
		glm::vec3 s0 = glm::vec3(Simulation::x[i], Simulation::z + 0.2f, Simulation::y[i]);
		float sr = 0.5f;

		float a = glm::dot(rd, rd);
		glm::vec3 s0_r0 = r0 - s0;
		float b = 2.0 * glm::dot(rd, s0_r0);
		float c = glm::dot(s0_r0, s0_r0) - (sr * sr);
		float disc = b * b - 4.0 * a * c;
		if (disc < 0.0) {
			//std::cout << "NO HIT" << std::endl;
		}
		else {
			hits[glm::distance(s0, camera.GetEye())] = i;
			std::cout << i << std::endl;
		}
	}
	if (!hits.empty()) Simulation::hit = hits.begin()->second;
}*/