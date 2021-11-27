#include "Object3Dvehicle.h"

Object3Dvehicle::Object3Dvehicle(ObjectStorage* objectStorage) : Object3D(objectStorage) {};
Object3Dvehicle::Object3Dvehicle(int id, std::string name, std::string type, std::string meshID, std::string textureID, std::string lightTexture, std::string breakTexture,
	std::string rightTexture, std::string leftTexture, std::string iconID,
	glm::vec3 initPosition, glm::vec3 initScale, glm::vec4 initRotation,
	glm::vec4 color, glm::vec4	hitSphere, glm::vec4 moveSphere, ObjectStorage* objectStorage)
	: Object3D(id, name, type, meshID, textureID, iconID, initPosition, initScale, initRotation, color, hitSphere, moveSphere, objectStorage) {
	this->lightTexture = lightTexture;
	this->breakTexture = breakTexture;
	this->rightTexture = rightTexture;
	this->leftTexture  = leftTexture;
}
Object3Dvehicle::~Object3Dvehicle(void) {}

Object3Dvehicle Object3Dvehicle::copy() {
	return Object3Dvehicle(this->id, this->name, this->type, this->meshID, this->textureID, this->lightTexture, this->breakTexture,
		this->rightTexture, this->leftTexture, this->iconID,
		this->position, this->scale, this->rotation,
		this->color, this->hitSphere, this->moveSphere, this->objectStorage);
}

Object3Dvehicle Object3Dvehicle::copy(int renderID) {
	Object3Dvehicle newObject = this->copy();
	newObject.setRenderID(renderID);
	return newObject;
}

float Object3Dvehicle::getMoveRtation(glm::vec3 pointA, glm::vec3 pointB) {
	glm::vec3 carDirection = glm::vec3(2, 0, 0);
	glm::vec3 roadDirection = pointB - pointA;
	bool flip;
	if (roadDirection.z > 0) {
		roadDirection = pointA - pointB;
		flip = true;
	}
	else {
		flip = false;
	}
	float angle = 0;
	//https://onlinemschool.com/math/library/vector/angl/
	angle = acos((glm::dot(carDirection, roadDirection)) / (glm::length(carDirection) * glm::length(roadDirection)));
	if (flip) angle -= M_PI;
	return  glm::degrees(angle);
}