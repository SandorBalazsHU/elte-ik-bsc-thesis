#pragma once

#include "Object3D.h"
#include "../Utilities/VertexArrayObject.h"
#include "../Utilities/BufferObject.h"
#include <vector>

class Object3Droad : public Object3D {
public:
	Object3Droad(void) {
		modelPointCount = 1.0f;
		basePoints[0].x = -5.0f;
		basePoints[0].y = roadYposition;
		basePoints[0].z = -0.0f;

		basePoints[1].x = -2.0f;
		basePoints[1].y = roadYposition;
		basePoints[1].z = 5.1f;

		basePoints[2].x = 2.0f;
		basePoints[2].y = roadYposition;
		basePoints[2].z = 5.1f;

		basePoints[3].x = 5.0f;
		basePoints[3].y = roadYposition;
		basePoints[3].z = -0.0f;
	};
	~Object3Droad(void) {};
	void generate();
	VertexArrayObject& getVAO() {
		return model;
	};

protected:
	void generateRoadPoints();
	glm::vec3 bezierPoint(float u);
	glm::vec3 shiftPoint(glm::vec3 point1, glm::vec3 point2, float currentShift);
	void clean();
	void fillModelCoordinates();
	void fillModelNormals();
	void fillModelTextures();
	void fillModelIndices();

	float shift = 4.0f;
	glm::vec3 basePoints[4];
	float roadYposition;
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> shiftedPoints;
	std::vector<glm::vec3> trackOne;
	std::vector<glm::vec3> trackTwo;

	size_t	modelPointCount;

	VertexArrayObject		model;
	ArrayBuffer				modelCoordinates;
	IndexBuffer				modelIndices;
	ArrayBuffer				modelNormals;
	ArrayBuffer				modelTextures;
};