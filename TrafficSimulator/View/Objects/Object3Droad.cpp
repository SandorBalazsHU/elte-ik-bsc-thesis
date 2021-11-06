//Tendi
#include "Object3Droad.h"
#include <glm/gtx/transform2.hpp>
#include <map>

Object3Droad::Object3Droad() {
	this->roadYposition = 0.1f;
	this->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
};

Object3Droad::Object3Droad(const Object3Droad&) {}

//Object3Droad::~Object3Droad(void) {}

int  Object3Droad::getRenderID() {
	return renderID;
}

void Object3Droad::setRenderID(int newRenderID) {
	renderID = newRenderID;
	roadYposition += renderID / 100.0f;
	std::cout << renderID << ". road " << roadYposition << " Position" << std::endl;
}

void Object3Droad::bind(Object3D* trackBall_0, Object3D* trackBall_1, Object3D* trackBall_2, Object3D* trackBall_3, Object3D* roadEndCircle_1, Object3D* roadEndCircle_2) {
	trackBall_0->setRGBcolor(glm::vec3(1, 0, 0));
	trackBall_0->setPosition(glm::vec3(-5.0f, roadYposition, 0.0f));
	trackBall_1->setRGBcolor(glm::vec3(0, 1, 0));
	trackBall_1->setPosition(glm::vec3(-2.0f, roadYposition, 5.1f));
	trackBall_2->setRGBcolor(glm::vec3(0, 0, 1));
	trackBall_2->setPosition(glm::vec3(2.0f, roadYposition, 5.1f));
	trackBall_3->setRGBcolor(glm::vec3(1, 1, 0));
	trackBall_3->setPosition(glm::vec3(5.0f, roadYposition, 0.0f));
	
	/*trackBall_0->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
	trackBall_1->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
	trackBall_2->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
	trackBall_3->setScale(glm::vec3(1.0f, 1.0f, 1.0f));*/

	trackBall_0->hide();
	trackBall_1->hide();
	trackBall_2->hide();
	trackBall_3->hide();

	trackBall_0->setProtection(true);
	trackBall_1->setProtection(true);
	trackBall_2->setProtection(true);
	trackBall_3->setProtection(true);

	this->trackBalls[0] = trackBall_0;
	this->trackBalls[1] = trackBall_1;
	this->trackBalls[2] = trackBall_2;
	this->trackBalls[3] = trackBall_3;

	this->updateBasePoints();
	generate();
	//TODO SHIT
	//roadEndCircle_1->setPosition(shiftPoint(points[0], points[1], shift/2.0f, roadYposition - 0.01f));
	//roadEndCircle_2->setPosition(shiftPoint(points[points.size()-2], points[points.size()-1], shift / 2.0f, roadYposition - 0.01f));
	roadEndCircle_1->setProtection(true);
	roadEndCircle_2->setProtection(true);
	roadEndCircle_1->setSelectable(false);
	roadEndCircle_2->setSelectable(false);
	this->roadEndCircles[0] = roadEndCircle_1;
	this->roadEndCircles[1] = roadEndCircle_2;
	//this->roadEndCircles[0]->setPosition(basePoints[0]);
	//this->roadEndCircles[1]->setPosition(basePoints[3]);
	this->roadEndCircles[0]->setPosition(glm::vec3(points[1].x, roadYposition - 0.01f, points[1].z));
	this->roadEndCircles[1]->setPosition(glm::vec3(points[points.size() - 1].x, roadYposition - 0.01f, points[points.size() - 1].z));

}

void Object3Droad::reBind(Object3D* trackBall_0, Object3D* trackBall_1, Object3D* trackBall_2, Object3D* trackBall_3, Object3D* roadEndCircle_1, Object3D* roadEndCircle_2) {
	this->trackBalls[0] = trackBall_0;
	this->trackBalls[1] = trackBall_1;
	this->trackBalls[2] = trackBall_2;
	this->trackBalls[3] = trackBall_3;
	this->updateBasePoints();
	this->roadEndCircles[0] = roadEndCircle_1;
	this->roadEndCircles[1] = roadEndCircle_2;
}

//TODO OTHER OBJECTS DELETE BUG
void Object3Droad::updateBasePoints() {
	if (this->stickMarkA != 'Q') {
		if (this->stickMarkA == 'A') {
			trackBalls[0]->setPosition(this->stickA->getEndpointA());
		}
		if (this->stickMarkA == 'B') {
			trackBalls[0]->setPosition(this->stickA->getEndpointB());
		}
	}

	if (this->stickMarkB != 'Q') {
		if (this->stickMarkB == 'A') {
			trackBalls[3]->setPosition(this->stickB->getEndpointA());
		}
		if (this->stickMarkB == 'B') {
			trackBalls[3]->setPosition(this->stickB->getEndpointB());
		}
	}

	for (size_t i = 0; i < 4; i++) {
		basePoints[i] = trackBalls[i]->getPosition();
		//std::cout << trackBalls[i]->getRenderID() << " | " << trackBalls[i]->getPosition().x << " , " << trackBalls[i]->getPosition().y << " , " << trackBalls[i]->getPosition().z << std::endl;
	}
	//std::cout << std::endl;
}

bool Object3Droad::isClicked(glm::vec3 cameraPosition, glm::vec3 ray) {
	for (size_t i = 1; i < points.size(); i++) {
		glm::vec3 hitSphereCenter = points[i];
		float hitSphereRadius = shift*2.0f;

		float a = glm::dot(ray, ray);
		glm::vec3 camToPointDirection = cameraPosition - hitSphereCenter;
		float b = 2.0 * glm::dot(ray, camToPointDirection);
		float c = glm::dot(camToPointDirection, camToPointDirection) - (hitSphereRadius * hitSphereRadius);
		float hitDistance = b * b - 4.0 * a * c;
		if (hitDistance < 0.0) {
			//std::cout << "NO HIT" << std::endl;
		}else{
			//std::cout << "ROAD INSIDE HIT" << std::endl;
			return true;
		}
	}
	return false;
}

void Object3Droad::setRGBAcolor(glm::vec4 RGBAcolor) {
	this->color = RGBAcolor;
}
void Object3Droad::setRGBcolor(glm::vec3 RGBcolor) {
	this->color.x = RGBcolor.x;
	this->color.y = RGBcolor.y;
	this->color.z = RGBcolor.z;
}
void Object3Droad::setOpacity(float opacity) {
	this->color.w = opacity;
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
	return this->color.w;
}

bool Object3Droad::isSelected() {
	return selected;
}

glm::vec3 Object3Droad::getEndpointA() {
	return basePoints[0];
}

glm::vec3 Object3Droad::getEndpointB() {
	return basePoints[3];
}

void Object3Droad::stuckTest(Object3Droad* road) {
	if (glm::distance(road->getEndpointA(), this->getEndpointA()) < this->shift) {
		this->stickA = road;
		this->stickMarkA = 'A';
	}
	if (glm::distance(road->getEndpointA(), this->getEndpointB()) < this->shift) {
		this->stickB = road;
		this->stickMarkB = 'A';
	}
	if (glm::distance(road->getEndpointB(), this->getEndpointA()) < this->shift) {
		this->stickA = road;
		this->stickMarkA = 'B';
	}
	if (glm::distance(road->getEndpointB(), this->getEndpointB()) < this->shift) {
		this->stickB = road;
		this->stickMarkB = 'B';
	}
}

void Object3Droad::select() {
	setOpacity(0.5f);
	roadEndCircles[0]->setOpacity(0.5f);
	roadEndCircles[1]->setOpacity(0.5f);
	trackBalls[0]->show();
	trackBalls[1]->show();
	trackBalls[2]->show();
	trackBalls[3]->show();
	selected = true;
}

void Object3Droad::deselect() {
	setOpacity(1.0f);
	roadEndCircles[0]->setOpacity(1.0f);
	roadEndCircles[1]->setOpacity(1.0f);
	trackBalls[0]->hide();
	trackBalls[1]->hide();
	trackBalls[2]->hide();
	trackBalls[3]->hide();
	selected = false;
}

void Object3Droad::generateRoadPoints() {
	for (double u = 0.0; u <= 1.0; u += 0.01) {
		points.push_back(bezierPoint(u));
	}

	shiftedPoints_1.push_back(shiftPoint(points[0], points[1], shift, roadYposition));
	shiftedPoints_2.push_back(shiftPoint(points[0], points[1], shift*-1.0f, roadYposition));
	trackOne.push_back(shiftPoint(points[0], points[1], (shift/2.0f), roadYposition));
	trackTwo.push_back(shiftPoint(points[0], points[1], ((shift/2.0f) * -1.0f), roadYposition));

	for (size_t i = 1; i < points.size(); i++) {
		shiftedPoints_1.push_back(shiftPoint(points[i-1], points[i], shift, roadYposition));
		shiftedPoints_2.push_back(shiftPoint(points[i-1], points[i], shift * -1.0f, roadYposition));
		trackOne.push_back(shiftPoint(points[i-1], points[i], (shift / 2.0f), roadYposition));
		trackTwo.push_back(shiftPoint(points[i-1], points[i], ((shift / 2.0f) * -1.0f), roadYposition));
	}
}

glm::vec3 Object3Droad::shiftPoint(glm::vec3 point1, glm::vec3 point2, float currentShift, float yPosition) {
	glm::vec3 shiftedPoint;
	float xnormal = -1 * (point2.z - point1.z);
	float znormal = (point2.x - point1.x);
	float normalLenght = sqrt(xnormal * xnormal + znormal * znormal);
	shiftedPoint.x = point2.x + ((xnormal / normalLenght) * currentShift);
	shiftedPoint.y = yPosition;
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
	shiftedPoints_1.clear();
	shiftedPoints_2.clear();
	trackOne.clear();
	trackTwo.clear();
}

void Object3Droad::fillModelCoordinates() {
	std::vector<glm::vec3> modelPoints;
	for (size_t i = 0; i < points.size(); i++) {
		modelPoints.push_back(shiftedPoints_2[i]);
		modelPoints.push_back(shiftedPoints_1[i]);
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

//TODO SHIT
void Object3Droad::update() {
	this->updateBasePoints();
	this->generate();
	this->roadEndCircles[0]->setPosition(glm::vec3(points[1].x, roadYposition - 0.01f, points[1].z));
	this->roadEndCircles[1]->setPosition(glm::vec3(points[points.size() - 1].x, roadYposition - 0.01f, points[points.size() - 1].z));
	//this->roadEndCircles[0]->setPosition(basePoints[0]);
	//this->roadEndCircles[1]->setPosition(basePoints[3]);
}