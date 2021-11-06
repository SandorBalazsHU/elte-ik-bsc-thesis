//T'ana
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

	void bind(Object3D* trackBall_0, Object3D* trackBall_1, Object3D* trackBall_2, Object3D* trackBall_3, Object3D* roadEndCircle_1, Object3D* roadEndCircle_2);
	void reBind(Object3D* trackBall_0, Object3D* trackBall_1, Object3D* trackBall_2, Object3D* trackBall_3, Object3D* roadEndCircle_1, Object3D* roadEndCircle_2);

	void generate();
	void update();
	VertexArrayObject& getVAO() {
		//updateBasePoints();
		return model;
	};

	Object3D** getTrackBalls() {
		return trackBalls;
	}

	int 					getRenderID();
	void					setRenderID(int newRenderID);
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
	bool					isClicked(glm::vec3 cameraPosition, glm::vec3 ray);
	glm::vec3				getEndpointA();
	glm::vec3				getEndpointB();
	//TODO: road delete rebind problem.
	void					stuckTest(Object3Droad* road);

	std::vector<glm::vec3> points;
	std::vector<glm::vec3> shiftedPoints_1;
	std::vector<glm::vec3> shiftedPoints_2;
	std::vector<glm::vec3> trackOne;
	std::vector<glm::vec3> trackTwo;

protected:
	void generateRoadPoints();
	glm::vec3 bezierPoint(float u);
	glm::vec3 shiftPoint(glm::vec3 point1, glm::vec3 point2, float currentShift, float yPosition);
	void updateBasePoints();
	void clean();

	void fillModelCoordinates();
	void fillModelNormals();
	void fillModelTextures();
	void fillModelIndices();

	int renderID = -1;
	bool selected = true;
	glm::vec4 color;

	Object3Droad* stickA = NULL;
	char stickMarkA = 'Q';
	Object3Droad* stickB = NULL;
	char stickMarkB = 'Q';

	float shift = 2.0f;
	Object3D* trackBalls[4];
	Object3D* roadEndCircles[2];
	glm::vec3 basePoints[4];
	float roadYposition;
	/*/std::vector<glm::vec3> points;
	std::vector<glm::vec3> shiftedPoints_1;
	std::vector<glm::vec3> shiftedPoints_2;
	std::vector<glm::vec3> trackOne;
	std::vector<glm::vec3> trackTwo;*/

	size_t	modelPointCount;

	VertexArrayObject		model;
	ArrayBuffer				modelCoordinates;
	IndexBuffer				modelIndices;
	ArrayBuffer				modelNormals;
	ArrayBuffer				modelTextures;
};