#pragma once
#include "Object3D.h"
#include "../Utilities/VertexArrayObject.h"
#include "../Utilities/BufferObject.h"
#include <vector>

class Object3Droad {
public:
	Object3Droad();
	Object3Droad(const Object3Droad&);
	~Object3Droad(void) {}

	void bind(Object3D* trackBall_0, Object3D* trackBall_1, Object3D* trackBall_2, Object3D* trackBall_3);

	void generate();
	VertexArrayObject& getVAO() {
		//updateBasePoints();
		return model;
	};

	/*Object3D* getTrackBalls() {
		return trackBalls;
	}*/

	void					setRGBAcolor(glm::vec4 RGBAcolor);
	void					setRGBcolor(glm::vec3 RGBcolor);
	void					setOpacity(float opacity);
	glm::mat4				getWorldMatrix();
	glm::vec4				getRGBAcolor();
	glm::vec3				getRGBcolor();
	float					getOpacity();
	bool					isSelected();
	void					select();
	void					deselect();

protected:
	void generateRoadPoints();
	glm::vec3 bezierPoint(float u);
	glm::vec3 shiftPoint(glm::vec3 point1, glm::vec3 point2, float currentShift);
	void updateBasePoints();
	void clean();

	void fillModelCoordinates();
	void fillModelNormals();
	void fillModelTextures();
	void fillModelIndices();

	bool selected = true;
	glm::vec4 color;

	float shift = 4.0f;
	Object3D* trackBalls[4];
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