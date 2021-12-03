//T'ana
#pragma once

class Render;

#include "Object3D.h"
#include "../Utilities/VertexArrayObject.h"
#include "../Utilities/BufferObject.h"
#include <vector>

class Object3Droad {
public:
	Object3Droad();
	Object3Droad(const Object3Droad&);
	~Object3Droad(void);

	void bind(Render* render, size_t newRenderID, size_t trackBall_0, size_t trackBall_1, size_t trackBall_2, size_t trackBall_3, size_t roadEndCircle_1, size_t roadEndCircle_2);
	void reBind(size_t trackBall_0, size_t trackBall_1, size_t trackBall_2, size_t trackBall_3, size_t roadEndCircle_1, size_t roadEndCircle_2);

	void generate();
	void update();
	VertexArrayObject& getVAO() {
		return model;
	};

	size_t* getTrackBalls() {
		return trackBalls;
	};

	size_t* getRoadEndCircles() {
		return roadEndCircles;
	};

	glm::vec3* getBasepoints() {
		return basepoints;
	};


	size_t 					getRenderID();
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
	void					stuckTest(size_t road);
	char					markerTest(size_t marker);
	void					setEndpointLock(bool lock);
	void					lockEditor();
	void					freeEditor();

	std::vector<glm::vec3> points;
	std::vector<glm::vec3> shiftedpoints_1;
	std::vector<glm::vec3> shiftedpoints_2;
	std::vector<glm::vec3> trackOne;
	std::vector<glm::vec3> trackTwo;

	size_t stickA = -1;
	char stickMarkA = 'Q';
	size_t stickB = -1;
	char stickMarkB = 'Q';

	size_t markerA = -1;
	size_t markerB = -1;

	size_t modelID = -1;
 
	float getRoadLength();

private:
	void generateRoadpoints();
	glm::vec3 bezierpoint(float u);
	glm::vec3 shiftpoint(glm::vec3 point1, glm::vec3 point2, float currentShift, float yPosition);
	void updateBasepoints();
	void clean();

	void endpointsExistTest();
	void markersExistTest();

	void fillModelCoordinates();
	void fillModelNormals();
	void fillModelTextures();
	void fillModelIndices();

	size_t renderID = -1;
	bool selected = true;
	bool endpointLock = true;
	
	glm::vec4 color;

	/*size_t stickA = -1;
	char stickMarkA = 'Q';
	size_t stickB = -1;
	char stickMarkB = 'Q';

	size_t markerA = -1;
	size_t markerB = -1;*/

	bool editorLock = false;

	float shift = 2.0f;
	size_t trackBalls[4];
	size_t roadEndCircles[2];
	glm::vec3 basepoints[4];
	float roadYposition;

	/*std::vector<glm::vec3> points;
	std::vector<glm::vec3> shiftedpoints_1;
	std::vector<glm::vec3> shiftedpoints_2;
	std::vector<glm::vec3> trackOne;
	std::vector<glm::vec3> trackTwo;*/

	size_t	modelpointCount;

	VertexArrayObject		model;
	ArrayBuffer				modelCoordinates;
	IndexBuffer				modelIndices;
	ArrayBuffer				modelNormals;
	ArrayBuffer				modelTextures;
	Render*					render;
};