/**
 * @name Traffic Simulation
 * @file Object3Dvehicle.cpp
 * @class Object3Dvehicle
 * @author Sándor Balázs - AZA6NL
 * @date 2021.12.15.
 * @brief The renderable vehicle representation.
 * Contact: sandorbalazs9402@gmail.com
*/

#include "Object3Dvehicle.h"

/**
 * @brief Constructor for the empty Vehicle object.
 * @param objectStorage The object storage pointer.
*/
Object3Dvehicle::Object3Dvehicle(ObjectStorage* objectStorage) : Object3D(objectStorage) {};

/**
 * @brief Constructor for copy and initialization. The data source is the configuration CSV.
 * @param id The vehicle ID.
 * @param name The vehicle name.
 * @param type The vehicle type. 
 * @param meshID The vehicle mesh ID.
 * @param textureID The vehicle texture ID.
 * @param lightTexture The vehicle lightTexture.
 * @param breakTexture The vehicle breakTexture.
 * @param rightTexture The vehicle rightTexture.
 * @param leftTexture The vehicle leftTexture.
 * @param iconID The vehicle iconID.
 * @param initPosition The vehicle initPosition.
 * @param initScale The vehicle initScale.
 * @param initRotation The vehicle initRotation.
 * @param color The vehicle color.
 * @param hitSphere The vehicle hitSphere.
 * @param moveSphere The vehicle moveSphere.
 * @param objectStorage The vehicle objectStorage.
*/
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

/**
 * @brief The vehicle destructor.
 * @param  Empty.
*/
Object3Dvehicle::~Object3Dvehicle(void) {}

/**
 * @brief Copy this object to an other and return it.
 * @return The copy from this object.
*/
Object3Dvehicle Object3Dvehicle::copy() {
	return Object3Dvehicle(this->id, this->name, this->type, this->meshID, this->textureID, this->lightTexture, this->breakTexture,
		this->rightTexture, this->leftTexture, this->iconID,
		this->position, this->scale, this->rotation,
		this->color, this->hitSphere, this->moveSphere, this->objectStorage);
}

/**
 * @brief Copy this object to an other and return it and set the render ID.
 * @param renderID The new render id.
 * @return The copy from this object.
*/
Object3Dvehicle Object3Dvehicle::copy(int renderID) {
	Object3Dvehicle newObject = this->copy();
	newObject.setRenderID(renderID);
	return newObject;
}

/**
 * @brief Calculate the vehicle rotation from the road current and next point.
 * @param pointA The current point of the road.
 * @param pointB The next point of the road.
 * @return The calculated direction.
*/
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
	angle = acos((glm::dot(carDirection, roadDirection)) / (glm::length(carDirection) * glm::length(roadDirection)));
	if (flip) angle -= M_PI;
	return  glm::degrees(angle);
}