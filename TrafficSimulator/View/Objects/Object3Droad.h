/**
 * @name Traffic Simulation
 * @file Object3Droad.cpp
 * @class Object3Droad
 * @author Sándor Balázs - AZA6NL
 * @date 2021.12.15.
 * @brief The parametric generated road implementation.
 * Contact: sandorbalazs9402@gmail.com
*/

//T'ana

#pragma once

//Empty lass for avoiding the circular includes.
class Render;

#include "Object3D.h"
#include "../Utilities/VertexArrayObject.h"
#include "../Utilities/BufferObject.h"
#include <vector>

/**
 * @brief The parametric generated road implementation.
*/
class Object3Droad {
public:

	/**
	 * @brief The Object3Droad constructor. Set the basic variables too.
	*/
	Object3Droad();

	/**
	 * @brief Object3Droad copy constructor.
	 * @param The copyable road.
	*/
	Object3Droad(const Object3Droad&);

	/**
	 * @brief Destructor.
	 * @param Empty.
	*/
	~Object3Droad(void);

	/**
	 * @brief Bind this road to the render and initialize the road, generate the 3D model and initialize the connected objects.
	 * @param render The current render pointer.
	 * @param newRenderID The current render ID.
	 * @param trackBall_0 The trackBall_0 object ID.
	 * @param trackBall_1 The trackBall_1 object ID.
	 * @param trackBall_2 The trackBall_2 object ID.
	 * @param trackBall_3 The trackBall_3 object ID.
	 * @param roadEndCircle_1 The roadEndCircle_1 object ID.
	 * @param roadEndCircle_2 The roadEndCircle_2 object ID.
	*/
	void bind(Render* render, size_t newRenderID, size_t trackBall_0, size_t trackBall_1, size_t trackBall_2, size_t trackBall_3, size_t roadEndCircle_1, size_t roadEndCircle_2);

	/**
	 * @brief Rebind the connected objects after changes.
	 * @param trackBall_0 The trackBall_0 object ID.
	 * @param trackBall_1 The trackBall_1 object ID.
	 * @param trackBall_2 The trackBall_2 object ID.
	 * @param trackBall_3 The trackBall_3 object ID.
	 * @param roadEndCircle_1 The roadEndCircle_1 object ID.
	 * @param roadEndCircle_2 The roadEndCircle_2 object ID.
	*/
	void reBind(size_t trackBall_0, size_t trackBall_1, size_t trackBall_2, size_t trackBall_3, size_t roadEndCircle_1, size_t roadEndCircle_2);

	/**
	 * @brief Generate the current 3D Mosel.
	*/
	void generate();

	/**
	 * @brief Update the road and recreate the model.
	*/
	void update();

	/**
	 * @brief Getter the renderable road VAO.
	 * @return The renderable road VAO.
	*/
	VertexArrayObject& getVAO();

	/**
	 * @brief Get the trackballs IDs.
	 * @return The trackballs IDs array.
	*/
	size_t* getTrackBalls();

	/**
	 * @brief Get the endpoint circles IDs.
	 * @return The endpoint circles IDs.
	*/
	size_t* getRoadEndCircles();

	/**
	 * @brief Get the base point coordinates.
	 * @return The base point coordinates in array.
	*/
	glm::vec3* getBasepoints();

	size_t getRenderID();

	void setRGBAcolor(glm::vec4 RGBAcolor);

	void setRGBcolor(glm::vec3 RGBcolor);

	void setOpacity(float opacity);

	glm::mat4 getWorldMatrix();

	glm::vec4 getRGBAcolor();


	glm::vec3 getRGBcolor();

	float getOpacity();

	bool isSelected();

	void select();

	void deselect();

	bool isClicked(glm::vec3 cameraPosition, glm::vec3 ray);

	glm::vec3 getEndpointA();

	glm::vec3 getEndpointB();

	void stuckTest(size_t road);

	char markerTest(size_t marker);

	void setEndpointLock(bool lock);

	void lockEditor();

	void freeEditor();

	float getRoadLength();

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

	bool editorLock = false;

	float shift = 2.0f;

	size_t trackBalls[4];

	size_t roadEndCircles[2];

	glm::vec3 basepoints[4];

	float roadYposition;

	size_t	modelpointCount;

	VertexArrayObject model;

	ArrayBuffer modelCoordinates;

	IndexBuffer modelIndices;

	ArrayBuffer modelNormals;

	ArrayBuffer modelTextures;

	Render* render;
};