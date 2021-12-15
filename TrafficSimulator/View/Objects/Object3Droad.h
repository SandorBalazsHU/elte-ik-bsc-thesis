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
	 * @brief Generate the current 3D Model.
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

	/**
	 * @brief Getter for the road render it.
	 * @return The road render it.
	*/
	size_t getRenderID();

	/**
	 * @brief Setter for the road render color with opacity.
	 * @param RGBAcolor The road render color  with opacity.
	*/
	void setRGBAcolor(glm::vec4 RGBAcolor);

	/**
	 * @brief Setter for the road render color without opacity.
	 * @param RGBcolor The road render color without opacity.
	*/
	void setRGBcolor(glm::vec3 RGBcolor);

	/**
	 * @brief Set road render opacity.
	 * @param opacity The road render opacity.
	*/
	void setOpacity(float opacity);

	/**
	 * @brief Calculate for the road world matrix for the road rendering.
	 * @return The road world matrix for the road rendering.
	*/
	glm::mat4 getWorldMatrix();

	/**
	 * @brief Getter for the road render color with opacity.
	 * @return The road render color with opacity.
	*/
	glm::vec4 getRGBAcolor();

	/**
	 * @brief Getter for the road render color without opacity.
	 * @return The road render color without opacity.
	*/
	glm::vec3 getRGBcolor();

	/**
	 * @brief Getter for the road opacity.
	 * @return The road opacity.
	*/
	float getOpacity();

	/**
	 * @brief Checker for the selected status flag.
	 * @return Return true if the road is in selected status.
	*/
	bool isSelected();

	/**
	 * @brief Set the road selected.
	*/
	void select();

	/**
	 * @brief Deselect the road.
	*/
	void deselect();

	/**
	 * @brief The road click checker. Return true if a ray intersects the road hit one of the spheres.
	 * @param cameraPosition The current camera position.
	 * @param ray The ray for the intersection.
	 * @return Return true if a ray intersects the road hit one of the spheres.
	*/
	bool isClicked(glm::vec3 cameraPosition, glm::vec3 ray);

	/**
	 * @brief Getter for the road Endpoint A position.
	 * @return The road Endpoint A position.
	*/
	glm::vec3 getEndpointA();

	/**
	 * @brief Getter for the road Endpoint B position.
	 * @return The road Endpoint B position.
	*/
	glm::vec3 getEndpointB();

	/**
	 * @brief Check the road endpoints sticking to an other road or endpoint marker.
	 * @param road
	*/
	void stuckTest(size_t road);

	/**
	 * @brief Testing the marker sticking to a road endpoint.
	 * @param marker The testable marker ID.
	 * @return Return the sticked endpoint name.
	*/
	char markerTest(size_t marker);

	/**
	 * @brief Set the endpoint lockers.
	 * @param lock Lock or free the endpoints.
	*/
	void setEndpointLock(bool lock);

	/**
	 * @brief Lock the editable mode.
	*/
	void lockEditor();

	/**
	 * @brief Free the editable mode.
	*/
	void freeEditor();

	/**
	 * @brief Calculate the road exact length.
	 * @return The road exact length.
	*/
	float getRoadLength();

	/**
	 * @brief The Bézier generated center road line.
	*/
	std::vector<glm::vec3> points;

	/**
	 * @brief The shifted side 1.
	*/
	std::vector<glm::vec3> shiftedpoints_1;

	/**
	 * @brief The shifted side 2.
	*/
	std::vector<glm::vec3> shiftedpoints_2;

	/**
	 * @brief The shifted track middle line 1.
	*/
	std::vector<glm::vec3> trackOne;

	/**
	 * @brief The shifted track middle line 2.
	*/
	std::vector<glm::vec3> trackTwo;

	/**
	 * @brief The endpoint A sticked id. -1 when not sticked.
	*/
	size_t stickA = -1;

	/**
	 * @brief To the endpoint A witch road endpoint joined? Q when not sticked.
	*/
	char stickMarkA = 'Q';

	/**
	 * @brief The endpoint a sticked id. -1 when not sticked.
	*/
	size_t stickB = -1;

	/**
	 * @brief To the endpoint B witch road endpoint joined? Q when not sticked.
	*/
	char stickMarkB = 'Q';

	/**
	 * @brief The endpoint A sticked marker. -1 when not sticked.
	*/
	size_t markerA = -1;

	/**
	 * @brief The endpoint B sticked marker. -1 when not sticked.
	*/
	size_t markerB = -1;

	/**
	 * @brief The abstract model ID.
	*/
	size_t modelID = -1;

private:

	/**
	 * @brief Generate the road one side points by a Bézier curve.
	*/
	void generateRoadpoints();

	/**
	 * @brief Generate a Bézier point bx u.
	 * @param u The parameter of the Bézier point.
	 * @return The generated Bézier point.
	*/
	glm::vec3 bezierpoint(float u);

	/**
	 * @brief Shift a point in Right angle to the point1 point2 section with currentShift in the yPosition level.
	 * @param point1  The section start point.
	 * @param point2 The section end point.
	 * @param currentShift The shift vector length.
	 * @param yPosition The Y level.
	 * @return The shifted point.
	*/
	glm::vec3 shiftpoint(glm::vec3 point1, glm::vec3 point2, float currentShift, float yPosition);

	/**
	 * @brief Update the base points position for the Bézier curve and do the endpoint and marker tests.
	*/
	void updateBasepoints();

	/**
	 * @brief Clean all the generated storages.
	*/
	void clean();

	/**
	 * @brief Test the endpoints validity.
	*/
	void endpointsExistTest();

	/**
	 * @brief Tester for the markers validity.
	*/
	void markersExistTest();

	/**
	 * @brief Fill the model coordinate storage.
	*/
	void fillModelCoordinates();

	/**
	 * @brief Fill the model normals.
	*/
	void fillModelNormals();

	/**
	 * @brief Fill the texture coordinates.
	*/
	void fillModelTextures();

	/**
	 * @brief Generate the 3D Vertexes.
	*/
	void fillModelIndices();

	/**
	 * @brief The render ID.
	*/
	size_t renderID = -1;

	/**
	 * @brief Is selected?
	*/
	bool selected = true;

	/**
	 * @brief The endpoint is locked?
	*/
	bool endpointLock = true;
	
	/**
	 * @brief The editor is locked?
	*/
	bool editorLock = false;

	/**
	 * @brief The road with/2. This is the road shift.
	*/
	float shift = 2.0f;

	/**
	 * @brief The road render color.
	*/
	glm::vec4 color;

	/**
	 * @brief The trackball ID-s array.
	*/
	size_t trackBalls[4];

	/**
	 * @brief The end circle ID-s array.
	*/
	size_t roadEndCircles[2];

	/**
	 * @brief The base points coordinates array.
	*/
	glm::vec3 basepoints[4];

	/**
	 * @brief The roads y height.
	*/
	float roadYposition;

	/**
	 * @brief The model points count.
	*/
	size_t	modelpointCount;

	/**
	 * @brief The model object.
	*/
	VertexArrayObject model;

	/**
	 * @brief The model coordinates.
	*/
	ArrayBuffer modelCoordinates;

	/**
	 * @brief The vertexes.
	*/
	IndexBuffer modelIndices;

	/**
	 * @brief The model normals.
	*/
	ArrayBuffer modelNormals;

	/**
	 * @brief The model texture coordinates.
	*/
	ArrayBuffer modelTextures;

	/**
	 * @brief The current render pointer.
	*/
	Render* render;
};