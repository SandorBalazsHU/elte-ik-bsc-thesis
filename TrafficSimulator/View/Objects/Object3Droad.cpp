#include "Object3Droad.h"

void Object3Droad::generateRoadPoints() {
	for (double u = 0.0; u <= 1.0; u += 0.01) {
		points.push_back(bezierPoint(u));
	}

	shiftedPoints.push_back(shiftPoint(points[0], points[1], shift));
	trackOne.push_back(shiftPoint(points[0], points[1], (shift * (1.0f / 4.0f))));
	trackTwo.push_back(shiftPoint(points[0], points[1], (shift * (3.0f / 4.0f))));

	for (size_t i = 1; i < points.size(); i++) {
		shiftedPoints.push_back(shiftPoint(points[i-1], points[i], shift));
		trackOne.push_back(shiftPoint(points[i-1], points[i], (shift * (1.0f / 4.0f))));
		trackTwo.push_back(shiftPoint(points[i-1], points[i], (shift * (3.0f / 4.0f))));
	}
}

glm::vec3 Object3Droad::shiftPoint(glm::vec3 point1, glm::vec3 point2, float currentShift) {
	glm::vec3 shiftedPoint;
	float xnormal = -1 * (point2.z - point1.z);
	float znormal = (point2.x - point1.x);
	float normalLenght = sqrt(xnormal * xnormal + znormal * znormal);
	shiftedPoint.x = point2.x + ((xnormal / normalLenght) * currentShift);
	shiftedPoint.y = roadYposition;
	shiftedPoint.z = point2.z + ((znormal / normalLenght) * currentShift);
	return shiftedPoint;
}

glm::vec3 Object3Droad::bezierPoint(float u) {
	glm::vec3 bezierPoint;
	bezierPoint.x = pow(1.0f - u, 3.0f) * basePoints[0].x + 3.0f * u * pow(1.0f - u, 2.0f) * basePoints[1].x
	+ 3.0f * pow(u, 2.0f) * (1.0f - u) * basePoints[2].x + pow(u, 3.0f) * basePoints[3].x;
	bezierPoint.y = roadYposition;
	bezierPoint.z = pow(1.0f - u, 3.0f) * basePoints[0].z + 3.0f * u * pow(1.0f - u, 2.0f) * basePoints[1].z
	+ 3.0f * pow(u, 2.0f) * (1.0f - u) * basePoints[2].z + pow(u, 3.0f) * basePoints[3].z;
	return bezierPoint;
}

void Object3Droad::clean() {
	points.clear();
	shiftedPoints.clear();
	trackOne.clear();
	trackTwo.clear();
}

void Object3Droad::fillModelCoordinates() {
	std::vector<glm::vec3> modelPoints;
	for (size_t i = 0; i < points.size(); i++) {
		modelPoints.push_back(points[i]);
		modelPoints.push_back(shiftedPoints[i]);
	}
	modelPointCount = modelPoints.size();
	modelCoordinates.BufferData(modelPoints);
}

void Object3Droad::fillModelNormals() {
	std::vector<glm::vec3> normals;
	for (size_t i = 0; i < modelPointCount; i++) {
		normals.push_back(glm::vec3(0, 1, 0));
	}
	modelNormals.BufferData(normals);
}

void Object3Droad::fillModelTextures() {
	std::vector<glm::vec2> textures;
	for (size_t i = 0; i < modelPointCount; i = i + 4) {
		textures.push_back(glm::vec2(1, 1));
		textures.push_back(glm::vec2(0, 1));
		textures.push_back(glm::vec2(1, 0));
		textures.push_back(glm::vec2(0, 0));
	}
	modelTextures.BufferData(textures);
}

void Object3Droad::fillModelIndices() {
	std::vector<int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);

	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(2);

	for (size_t i = 6; i <= modelPointCount * 3; i = i + 6) {
		indices.push_back(indices[i - 6] + 2);
		indices.push_back(indices[i - 5] + 2);
		indices.push_back(indices[i - 4] + 2);
		indices.push_back(indices[i - 3] + 2);
		indices.push_back(indices[i - 2] + 2);
		indices.push_back(indices[i - 1] + 2);
	}
	modelIndices.BufferData(indices);
}


void Object3Droad::generate() {
	clean();
	generateRoadPoints();
	fillModelCoordinates();
	fillModelNormals();
	fillModelTextures();
	fillModelIndices();
	model.Init({
		{ CreateAttribute<0, glm::vec3, 0, sizeof(glm::vec3)>, modelCoordinates },
		{ CreateAttribute<1, glm::vec3, 0, sizeof(glm::vec3)>, modelNormals },
		{ CreateAttribute<2, glm::vec2, 0, sizeof(glm::vec2)>, modelTextures }
	}, modelIndices);
}