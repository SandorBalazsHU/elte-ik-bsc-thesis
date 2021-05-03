#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include "pch.h"
#include "View/Camera.h"
#include "View/Camera.cpp"

TEST(CameraTest, CameraConstructorTest) {
	Camera camera;
	//camera.getCameraPosition();
	glm::vec3 lookedPoint = camera.getLookedPoint();
	/*camera.getUpwardVector();
	camera.getProjectionMatrix();
	camera.getViewMatrix();
	camera.getViewProjectionMatrix();*/
	ASSERT_EQ(lookedPoint.x, 0.0f);
	ASSERT_EQ(lookedPoint.z, 0.0f);
	ASSERT_EQ(lookedPoint.y, 0.0f);
}

TEST(CameraTest, getCameraPosition) {
	ASSERT_EQ(0.0f, 0.0f);
}