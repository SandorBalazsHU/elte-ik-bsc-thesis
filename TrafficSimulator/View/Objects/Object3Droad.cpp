/**
 * @name Traffic Simulation
 * @file Object3Droad.cpp
 * @class Object3Droad
 * @author S�ndor Bal�zs - AZA6NL
 * @date 2021.12.15.
 * @brief The parametric generated road implementation.
 * Contact: sandorbalazs9402@gmail.com
*/

//Tendi

#include "Object3Droad.h"
#include "../Render.h"
#include <glm/gtx/transform2.hpp>
#include <map>

/**
 * @brief The Object3Droad constructor. Set the basic variables too.
*/
Object3Droad::Object3Droad() {
	roadYposition = 0.1f;
	color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
};

/**
 * @brief Object3Droad copy constructor.
 * @param The copyable road.
*/
Object3Droad::Object3Droad(const Object3Droad&) {}

/**
 * @brief Destructor.
 * @param Empty.
*/
Object3Droad::~Object3Droad(void) {}

/**
 * @brief Getter for the road render it.
 * @return The road render it.
*/
size_t  Object3Droad::getRenderID() {
	return renderID;
}

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
void Object3Droad::bind(Render* render, size_t newRenderID, size_t trackBall_0, size_t trackBall_1, size_t trackBall_2, size_t trackBall_3, size_t roadEndCircle_1, size_t roadEndCircle_2) {
	this->render = render;
	this->renderID = newRenderID;
	this->roadYposition += renderID / 100.0f;

	trackBalls[0] = trackBall_0;
	trackBalls[1] = trackBall_1;
	trackBalls[2] = trackBall_2;
	trackBalls[3] = trackBall_3;
	roadEndCircles[0] = roadEndCircle_1;
	roadEndCircles[1] = roadEndCircle_2;

	for (size_t i = 0; i < 4; i++) {
		render->getObject(trackBalls[i])->setDependencyID(renderID);
		render->getObject(trackBalls[i])->hide();
		render->getObject(trackBalls[i])->setProtection(true);
	}
	for (size_t i = 0; i < 2; i++) {
		render->getObject(roadEndCircles[i])->setDependencyID(renderID);
	}
	for (size_t i = 0; i < 2; i++) {
		render->getObject(roadEndCircles[i])->setProtection(true);
		render->getObject(roadEndCircles[i])->setSelectable(false);
	}

	render->getObject(trackBalls[0])->setRGBcolor(glm::vec3(1.0f, 0.0f, 0.0f));
	render->getObject(trackBalls[1])->setRGBcolor(glm::vec3(0.0f, 1.0f, 0.0f));
	render->getObject(trackBalls[2])->setRGBcolor(glm::vec3(0.0f, 0.0f, 1.0f));
	render->getObject(trackBalls[3])->setRGBcolor(glm::vec3(1.0f, 1.0f, 0.0f));
	render->getObject(trackBalls[0])->setPosition(glm::vec3(50.0f, roadYposition, -5.0f));
	render->getObject(trackBalls[1])->setPosition(glm::vec3(53.0f, roadYposition, 7.0f));
	render->getObject(trackBalls[2])->setPosition(glm::vec3(57.0f, roadYposition, 7.0f));
	render->getObject(trackBalls[3])->setPosition(glm::vec3(60.0f, roadYposition, -5.0f));
	updateBasepoints();
	generate();
	render->getObject(roadEndCircles[0])->setPosition(glm::vec3(points[1].x, roadYposition + 0.05f, points[1].z));
	render->getObject(roadEndCircles[1])->setPosition(glm::vec3(points[points.size() - 1].x, roadYposition + 0.05f, points[points.size() - 1].z));
}

/**
 * @brief Rebind the connected objects after changes. 
 * @param trackBall_0 The trackBall_0 object ID.
 * @param trackBall_1 The trackBall_1 object ID.
 * @param trackBall_2 The trackBall_2 object ID.
 * @param trackBall_3 The trackBall_3 object ID.
 * @param roadEndCircle_1 The roadEndCircle_1 object ID.
 * @param roadEndCircle_2 The roadEndCircle_2 object ID.
*/
void Object3Droad::reBind(size_t trackBall_0, size_t trackBall_1, size_t trackBall_2, size_t trackBall_3, size_t roadEndCircle_1, size_t roadEndCircle_2) {
	trackBalls[0] = trackBall_0;
	trackBalls[1] = trackBall_1;
	trackBalls[2] = trackBall_2;
	trackBalls[3] = trackBall_3;
	roadEndCircles[0] = roadEndCircle_1;
	roadEndCircles[1] = roadEndCircle_2;
	updateBasepoints();
}

/**
 * @brief Update the base points position for the B�zier curve and do the endpoint and marker tests.
*/
void Object3Droad::updateBasepoints() {
	endpointsExistTest();
	markersExistTest();
	if (stickMarkA != 'Q' && endpointLock) {
		if (stickMarkA == 'A') render->getObject(trackBalls[0])->setPosition(render->getDynamicObject(stickA)->getEndpointA());
		if (stickMarkA == 'B') render->getObject(trackBalls[0])->setPosition(render->getDynamicObject(stickA)->getEndpointB());
	}

	if (stickMarkB != 'Q' && endpointLock) {
		if (stickMarkB == 'A') render->getObject(trackBalls[3])->setPosition(render->getDynamicObject(stickB)->getEndpointA());
		if (stickMarkB == 'B') render->getObject(trackBalls[3])->setPosition(render->getDynamicObject(stickB)->getEndpointB());
	}

	for (size_t i = 0; i < 4; i++) basepoints[i] = render->getObject(trackBalls[i])->getPosition();
}

//TODO Update the hit checker version
/**
 * @brief The road click checker. Return true if a ray intersects the road hit one of the spheres.
 * @param cameraPosition The current camera position.
 * @param ray The ray for the intersection.
 * @return Return true if a ray intersects the road hit one of the spheres.
*/
bool Object3Droad::isClicked(glm::vec3 cameraPosition, glm::vec3 ray) {
	for (size_t i = 1; i < points.size(); i++) {
		glm::vec3 hitSphereCenter = points[i];
		float hitSphereRadius = shift*2.0f;

		float a = glm::dot(ray, ray);
		glm::vec3 camToPointDirection = cameraPosition - hitSphereCenter;
		float b = 2.0f * glm::dot(ray, camToPointDirection);
		float c = glm::dot(camToPointDirection, camToPointDirection) - (hitSphereRadius * hitSphereRadius);
		float hitDistance = b * b - 4.0f * a * c;
		if (hitDistance >= 0.0) {
			return true;
		}
	}
	return false;
}

/**
 * @brief Setter for the road render color with opacity.
 * @param RGBAcolor The road render color  with opacity.
*/
void Object3Droad::setRGBAcolor(glm::vec4 RGBAcolor) {
	color = RGBAcolor;
}

/**
 * @brief Setter for the road render color without opacity.
 * @param RGBcolor The road render color without opacity.
*/
void Object3Droad::setRGBcolor(glm::vec3 RGBcolor) {
	color.x = RGBcolor.x;
	color.y = RGBcolor.y;
	color.z = RGBcolor.z;
	render->getObject(roadEndCircles[0])->setRGBcolor(RGBcolor);
	render->getObject(roadEndCircles[1])->setRGBcolor(RGBcolor);
}

/**
 * @brief Set road render opacity.
 * @param opacity The road render opacity.
*/
void Object3Droad::setOpacity(float opacity) {
	color.w = opacity;
}

/**
 * @brief Calculate for the road world matrix for the road rendering.
 * @return The road world matrix for the road rendering.
*/
glm::mat4 Object3Droad::getWorldMatrix() {
	return glm::translate(glm::vec3(0, 0, 0));
}

/**
 * @brief Getter for the road render color with opacity.
 * @return The road render color with opacity.
*/
glm::vec4 Object3Droad::getRGBAcolor() {
	return color;
}

/**
 * @brief Getter for the road render color without opacity.
 * @return The road render color without opacity.
*/
glm::vec3 Object3Droad::getRGBcolor() {
	return glm::vec3(color.y, color.z, color.w);
}

/**
 * @brief Getter for the road opacity.
 * @return The road opacity.
*/
float Object3Droad::getOpacity() {
	return color.w;
}

/**
 * @brief Checker for the selected status flag.
 * @return Return true if the road is in selected status.
*/
bool Object3Droad::isSelected() {
	return selected;
}

/**
 * @brief Calculate the road exact length.
 * @return The road exact length.
*/
float Object3Droad::getRoadLength() {
	float length = 0.0f;
	for (size_t i = 1; i < points.size(); i++) {
		length += glm::distance(points[i-1], points[i]);
	}
	return length;
}

/**
 * @brief Getter for the road Endpoint A position.
 * @return The road Endpoint A position.
*/
glm::vec3 Object3Droad::getEndpointA() {
	return basepoints[0];
}

/**
 * @brief Getter for the road Endpoint B position.
 * @return The road Endpoint B position.
*/
glm::vec3 Object3Droad::getEndpointB() {
	return basepoints[3];
}

/**
 * @brief Test the endpoints validity.
*/
void Object3Droad::endpointsExistTest() {
	if (render->getDynamicObject(stickA) == NULL) {
		stickA == -1;
		stickMarkA = 'Q';
	}
	if (render->getDynamicObject(stickB) == NULL) {
		stickB == -1;
		stickMarkB = 'Q';
	}
}

/**
 * @brief Tester for the markers validity.
*/
void Object3Droad::markersExistTest() {
	if (render->getDynamicObject(markerA) == NULL) {
		markerA == -1;
	}
	if (render->getDynamicObject(markerB) == NULL) {
		markerB == -1;
	}
}

/**
 * @brief Check the road endpoints sticking to an other road or endpoint marker.
 * @param road 
*/
void Object3Droad::stuckTest(size_t road) {
	endpointsExistTest();
	if(glm::distance(render->getDynamicObject(road)->getEndpointA(), getEndpointA()) < shift + 1) {
		stickA = road;
		stickMarkA = 'A';
	}
	if (glm::distance(render->getDynamicObject(road)->getEndpointA(), getEndpointB()) < shift + 1) {
		stickB = road;
		stickMarkB = 'A';
	}
	if (glm::distance(render->getDynamicObject(road)->getEndpointB(), getEndpointA()) < shift + 1) {
		stickA = road;
		stickMarkA = 'B';
	}
	if (glm::distance(render->getDynamicObject(road)->getEndpointB(), getEndpointB()) < shift + 1) {
		stickB = road;
		stickMarkB = 'B';
	}
}

/**
 * @brief Testing the marker sticking to a road endpoint.
 * @param marker The testable marker ID.
 * @return Return the sticked endpoint name.
*/
char Object3Droad::markerTest(size_t marker) {
	markersExistTest();
	char result = 'Q';
	if ((glm::distance(getEndpointA(), render->getObject(marker)->getPosition()) < shift + 1) && endpointLock) {
		result = 'A';
		markerA = marker;
		if (!editorLock) render->getObject(marker)->setSelectable(false);
		render->getObject(marker)->setPosition(getEndpointA());
	}
	if ((glm::distance(getEndpointB(), render->getObject(marker)->getPosition()) < shift + 1) && endpointLock) {
		result = 'B';
		markerB = marker;
		if (!editorLock) render->getObject(marker)->setSelectable(false);
		render->getObject(marker)->setPosition(getEndpointB());
	}
	return result;
}

/**
 * @brief Set the road selected.
*/
void Object3Droad::select() {
	setOpacity(0.5f);
	for (size_t i = 0; i < 2; i++) render->getObject(roadEndCircles[i])->setOpacity(0.5f);
	if(!render->isEditorLoced()) for (size_t i = 0; i < 4; i++) render->getObject(trackBalls[i])->show();
	selected = true;
}

/**
 * @brief Deselect the road.
*/
void Object3Droad::deselect() {
	setOpacity(1.0f);
	for (size_t i = 0; i < 2; i++) render->getObject(roadEndCircles[i])->setOpacity(1.0f);
	for (size_t i = 0; i < 4; i++) render->getObject(trackBalls[i])->hide();
	selected = false;
}

/**
 * @brief Generate the road one side points by a B�zier curve.
*/
void Object3Droad::generateRoadpoints() {
	for (double u = 0.0; u <= 1.0; u += 0.01) {
		points.push_back(this->bezierpoint(u));
	}

	shiftedpoints_1.push_back(shiftpoint(points[0], points[1], shift, roadYposition));
	this->shiftedpoints_2.push_back(shiftpoint(points[0], points[1], shift*-1.0f, roadYposition));
	trackOne.push_back(shiftpoint(points[0], points[1], (shift/2.0f), roadYposition));
	trackTwo.push_back(shiftpoint(points[0], points[1], ((shift/2.0f) * -1.0f), roadYposition));

	for (size_t i = 1; i < points.size(); i++) {
		shiftedpoints_1.push_back(shiftpoint(points[i-1], points[i], shift, roadYposition));
		shiftedpoints_2.push_back(shiftpoint(points[i-1], points[i], shift * -1.0f, roadYposition));
		trackOne.push_back(shiftpoint(points[i-1], points[i], (shift / 2.0f), roadYposition));
		trackTwo.push_back(this->shiftpoint(points[i-1], points[i], ((shift / 2.0f) * -1.0f), roadYposition));
	}
}

/**
 * @brief Shift a point in Right angle to the point1 point2 section with currentShift in the yPosition level.
 * @param point1  The section start point.
 * @param point2 The section end point.
 * @param currentShift The shift vector length.
 * @param yPosition The Y level.
 * @return The shifted point.
*/
glm::vec3 Object3Droad::shiftpoint(glm::vec3 point1, glm::vec3 point2, float currentShift, float yPosition) {
	glm::vec3 shiftedPoint;
	float xnormal = -1 * (point2.z - point1.z);
	float znormal = (point2.x - point1.x);
	float normalLenght = sqrt(xnormal * xnormal + znormal * znormal);
	shiftedPoint.x = point2.x + ((xnormal / normalLenght) * currentShift);
	shiftedPoint.y = yPosition;
	shiftedPoint.z = point2.z + ((znormal / normalLenght) * currentShift);
	return shiftedPoint;
}

/**
 * @brief Generate a B�zier point bx u.
 * @param u The parameter of the B�zier point.
 * @return The generated B�zier point.
*/
glm::vec3 Object3Droad::bezierpoint(float u) {
	glm::vec3 bezierPoint;
	bezierPoint.x = pow(1.0f - u, 3.0f) * basepoints[0].x + 3.0f * u * pow(1.0f - u, 2.0f) * basepoints[1].x
	+ 3.0f * pow(u, 2.0f) * (1.0f - u) * basepoints[2].x + pow(u, 3.0f) * basepoints[3].x;
	bezierPoint.y = roadYposition;
	bezierPoint.z = pow(1.0f - u, 3.0f) * basepoints[0].z + 3.0f * u * pow(1.0f - u, 2.0f) * basepoints[1].z
	+ 3.0f * pow(u, 2.0f) * (1.0f - u) * basepoints[2].z + pow(u, 3.0f) * basepoints[3].z;
	return bezierPoint;
}

/**
 * @brief Clean all the generated storages.
*/
void Object3Droad::clean() {
	points.clear();
	shiftedpoints_1.clear();
	shiftedpoints_2.clear();
	trackOne.clear();
	trackTwo.clear();
}

/**
 * @brief Fill the model coordinate storage.
*/
void Object3Droad::fillModelCoordinates() {
	std::vector<glm::vec3> modelPoints;
	for (size_t i = 0; i < points.size(); i++) {
		modelPoints.push_back(shiftedpoints_2[i]);
		modelPoints.push_back(shiftedpoints_1[i]);
	}
	modelpointCount = modelPoints.size();
	modelCoordinates.BufferData(modelPoints);
}

/**
 * @brief Fill the model normals.
*/
void Object3Droad::fillModelNormals() {
	std::vector<glm::vec3> normals;
	for (size_t i = 0; i < modelpointCount; i++) {
		normals.push_back(glm::vec3(0, 1, 0));
	}
	modelNormals.BufferData(normals);
}

/**
 * @brief Fill the texture coordinates.
*/
void Object3Droad::fillModelTextures() {
	std::vector<glm::vec2> textures;
	for (size_t i = 0; i < modelpointCount; i = i + 4) {
		textures.push_back(glm::vec2(1, 1));
		textures.push_back(glm::vec2(0, 1));
		textures.push_back(glm::vec2(1, 0));
		textures.push_back(glm::vec2(0, 0));
	}
	modelTextures.BufferData(textures);
}

/**
 * @brief Generate the 3D Vertexes.
*/
void Object3Droad::fillModelIndices() {
	std::vector<int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);

	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(2);

	for (size_t i = 6; i <= modelpointCount * 3; i = i + 6) {
		indices.push_back(indices[i - 6] + 2);
		indices.push_back(indices[i - 5] + 2);
		indices.push_back(indices[i - 4] + 2);
		indices.push_back(indices[i - 3] + 2);
		indices.push_back(indices[i - 2] + 2);
		indices.push_back(indices[i - 1] + 2);
	}
	modelIndices.BufferData(indices);
}

/**
 * @brief Generate the current 3D Mosel.
*/
void Object3Droad::generate() {
	clean();
	generateRoadpoints();
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

/**
 * @brief Update the road and recreate the model.
*/
void Object3Droad::update() {
	updateBasepoints();
	generate();
	render->getObject(roadEndCircles[0])->setPosition(glm::vec3(points[1].x, roadYposition + 0.05f, points[1].z));
	render->getObject(roadEndCircles[1])->setPosition(glm::vec3(points[points.size() - 1].x, roadYposition + 0.05f, points[points.size() - 1].z));
}

/**
 * @brief Set the endpoint lockers.
 * @param lock Lock or free the endpoints.
*/
void Object3Droad::setEndpointLock(bool lock) {
	endpointLock = lock;
	stickA = -1;
	stickMarkA = 'Q';
	stickB = -1;
	stickMarkB = 'Q';
	if (markerA != -1) render->getObject(markerA)->setSelectable(true);
	if (markerB != -1) render->getObject(markerB)->setSelectable(true);
	markerA = -1;
	markerB = -1;
}

/**
 * @brief Lock the editable mode.
*/
void Object3Droad::lockEditor() {
	this->editorLock = true;
	if (this->markerA != -1) this->render->getObject(this->markerA)->setSelectable(true);
	if (this->markerB != -1) this->render->getObject(this->markerB)->setSelectable(true);
}

/**
 * @brief Free the editable mode.
*/
void Object3Droad::freeEditor() {
	this->editorLock = false;
}

/**
 * @brief Getter the renderable road VAO.
 * @return The renderable road VAO.
*/
VertexArrayObject& Object3Droad::getVAO() {
	return model;
};

/**
 * @brief Get the trackballs IDs.
 * @return The trackballs IDs array.
*/
size_t* Object3Droad::getTrackBalls() {
	return trackBalls;
};

/**
 * @brief Get the endpoint circles IDs.
 * @return The endpoint circles IDs.
*/
size_t* Object3Droad::getRoadEndCircles() {
	return roadEndCircles;
};

/**
 * @brief Get the base point coordinates.
 * @return The base point coordinates in array.
*/
glm::vec3* Object3Droad::getBasepoints() {
	return basepoints;
};