//Tendi
#include "Object3Droad.h"
#include "../Render.h"
#include <glm/gtx/transform2.hpp>
#include <map>

Object3Droad::Object3Droad() {
	roadYposition = 0.1f;
	color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
};

Object3Droad::Object3Droad(const Object3Droad&) {}

Object3Droad::~Object3Droad(void) {
	//std::cout << "DESTRUCTOR" << std::endl;
}

size_t  Object3Droad::getRenderID() {
	return renderID;
}

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

void Object3Droad::reBind(size_t trackBall_0, size_t trackBall_1, size_t trackBall_2, size_t trackBall_3, size_t roadEndCircle_1, size_t roadEndCircle_2) {
	trackBalls[0] = trackBall_0;
	trackBalls[1] = trackBall_1;
	trackBalls[2] = trackBall_2;
	trackBalls[3] = trackBall_3;
	roadEndCircles[0] = roadEndCircle_1;
	roadEndCircles[1] = roadEndCircle_2;
	updateBasepoints();
}

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

//TODO Áthozni a felújított verziót.
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

void Object3Droad::setRGBAcolor(glm::vec4 RGBAcolor) {
	color = RGBAcolor;
}
//TODO A lemezekre is átvinni a változásokat
void Object3Droad::setRGBcolor(glm::vec3 RGBcolor) {
	color.x = RGBcolor.x;
	color.y = RGBcolor.y;
	color.z = RGBcolor.z;
	render->getObject(roadEndCircles[0])->setRGBcolor(RGBcolor);
	render->getObject(roadEndCircles[1])->setRGBcolor(RGBcolor);
}
void Object3Droad::setOpacity(float opacity) {
	color.w = opacity;
}

glm::mat4 Object3Droad::getWorldMatrix() {
	return glm::translate(glm::vec3(0, 0, 0));
}

glm::vec4 Object3Droad::getRGBAcolor() {
	return color;
}

glm::vec3 Object3Droad::getRGBcolor() {
	return glm::vec3(color.y, color.z, color.w);
}

float Object3Droad::getOpacity() {
	return color.w;
}

bool Object3Droad::isSelected() {
	return selected;
}

float Object3Droad::getRoadLength() {
	float length = 0.0f;
	for (size_t i = 1; i < points.size(); i++) {
		length += glm::distance(points[i-1], points[i]);
	}
	return length;
}

glm::vec3 Object3Droad::getEndpointA() {
	//Törlés utáni endpoint mozgatás index hibát okoz.
	return basepoints[0];
}

glm::vec3 Object3Droad::getEndpointB() {
	return basepoints[3];
}

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

void Object3Droad::markersExistTest() {
	if (render->getDynamicObject(markerA) == NULL) {
		markerA == -1;
	}
	if (render->getDynamicObject(markerB) == NULL) {
		markerB == -1;
	}
}

void Object3Droad::stuckTest(size_t road) {
	//std::cout << "RUNNED" << std::endl;
	//TODO Javasolt endpoint is deleted, marker is deleted hozzáadása ami törli a hibás út függõségeket.
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

char  Object3Droad::markerTest(size_t marker) {
	markersExistTest();
	char result = 'Q';
	if ((glm::distance(getEndpointA(), render->getObject(marker)->getPosition()) < shift + 1) && endpointLock) {
		result = 'A';
		markerA = marker;
		render->getObject(marker)->setSelectable(false);
		render->getObject(marker)->setPosition(getEndpointA());
	}
	if ((glm::distance(getEndpointB(), render->getObject(marker)->getPosition()) < shift + 1) && endpointLock) {
		result = 'B';
		markerB = marker;
		render->getObject(marker)->setSelectable(false);
		render->getObject(marker)->setPosition(getEndpointB());
	}
	return result;
}

void Object3Droad::select() {
	setOpacity(0.5f);
	for (size_t i = 0; i < 2; i++) render->getObject(roadEndCircles[i])->setOpacity(0.5f);
	if(!render->isEditorLoced()) for (size_t i = 0; i < 4; i++) render->getObject(trackBalls[i])->show();
	selected = true;
}

void Object3Droad::deselect() {
	setOpacity(1.0f);
	for (size_t i = 0; i < 2; i++) render->getObject(roadEndCircles[i])->setOpacity(1.0f);
	for (size_t i = 0; i < 4; i++) render->getObject(trackBalls[i])->hide();
	selected = false;
}

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

glm::vec3 Object3Droad::bezierpoint(float u) {
	glm::vec3 bezierPoint;
	bezierPoint.x = pow(1.0f - u, 3.0f) * basepoints[0].x + 3.0f * u * pow(1.0f - u, 2.0f) * basepoints[1].x
	+ 3.0f * pow(u, 2.0f) * (1.0f - u) * basepoints[2].x + pow(u, 3.0f) * basepoints[3].x;
	bezierPoint.y = roadYposition;
	bezierPoint.z = pow(1.0f - u, 3.0f) * basepoints[0].z + 3.0f * u * pow(1.0f - u, 2.0f) * basepoints[1].z
	+ 3.0f * pow(u, 2.0f) * (1.0f - u) * basepoints[2].z + pow(u, 3.0f) * basepoints[3].z;
	return bezierPoint;
}

void Object3Droad::clean() {
	points.clear();
	shiftedpoints_1.clear();
	shiftedpoints_2.clear();
	trackOne.clear();
	trackTwo.clear();
}

void Object3Droad::fillModelCoordinates() {
	std::vector<glm::vec3> modelPoints;
	for (size_t i = 0; i < points.size(); i++) {
		modelPoints.push_back(shiftedpoints_2[i]);
		modelPoints.push_back(shiftedpoints_1[i]);
	}
	modelpointCount = modelPoints.size();
	modelCoordinates.BufferData(modelPoints);
}

void Object3Droad::fillModelNormals() {
	std::vector<glm::vec3> normals;
	for (size_t i = 0; i < modelpointCount; i++) {
		normals.push_back(glm::vec3(0, 1, 0));
	}
	modelNormals.BufferData(normals);
}

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

void Object3Droad::update() {
	//std::cout << "UPDATED " << getRenderID() << std::endl;
	updateBasepoints();
	generate();
	render->getObject(roadEndCircles[0])->setPosition(glm::vec3(points[1].x, roadYposition + 0.05f, points[1].z));
	render->getObject(roadEndCircles[1])->setPosition(glm::vec3(points[points.size() - 1].x, roadYposition + 0.05f, points[points.size() - 1].z));
}

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