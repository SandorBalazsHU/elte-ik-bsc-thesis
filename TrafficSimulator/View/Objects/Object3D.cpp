#include "Object3D.h"
#include "ObjectStorage.h"
#include <glm/gtx/transform2.hpp>

Object3D::Object3D(std::string name, std::string type, std::string meshID, std::string textureID, std::string iconID,
	glm::vec3 initPosition, glm::vec3 initScale, glm::vec4 initRotation,
	glm::vec4 color, glm::vec4	hitSphere, ObjectStorage* objectStorage) {
	this->name = name;
	this->type = type;
	this->meshID = meshID;
	this->textureID = textureID;
	this->iconID = iconID;
	this->initPosition = initPosition;
	this->initScale = initScale;
	this->initRotation = initRotation;
	this->color = color;
	this->hitSphere = hitSphere;
	this->objectStorage = objectStorage;
}

void Object3D::setMeshID(std::string meshID) {
	this->meshID = meshID;
}

void Object3D::setTextureID(std::string textureID) {
	this->textureID = textureID;
}

void Object3D::setIconID(std::string iconID) {
	this->iconID = iconID;
}

void Object3D::setPosition(glm::vec3& position) {
	this->position = position;
}
void Object3D::setRotation(glm::vec4& rotation) {
	this->rotation = rotation;
}

void Object3D::setScale(glm::vec3& scale) {
	this->scale = scale;
}

void Object3D::move(glm::vec3& move) {
	this->position += move;
}
void Object3D::rotateLeft(float& rotateLeft) {
	this->rotation += glm::vec4(rotateLeft, 0, 0, 0);
}

void Object3D::rotateRight(float& rotateRight) {
	this->rotation += glm::vec4(-rotateRight, 0, 0, 0);
}
void Object3D::setRGBAcolor(glm::vec4& RGBAcolor) {
	this->color = RGBAcolor;
}

void Object3D::setRGBcolor(glm::vec3& RGBcolor) {
	this->color.x = RGBcolor.x;
	this->color.y = RGBcolor.y;
	this->color.z = RGBcolor.z;
}

void Object3D::setOpacity(float& opacity) {
	this->color.w = opacity;
}

std::unique_ptr<Mesh>& Object3D::getMesh() {
	return objectStorage->getObject(this->meshID);
}

Texture2D& Object3D::getTexture() {
	return objectStorage->getTexture(this->textureID);
}

std::string Object3D::getTextureID() {
	return this->textureID;
}

Texture2D& Object3D::getIcon() {
	return objectStorage->getTexture(this->iconID);
}

std::string Object3D::getIconID() {
	return this->iconID;
}

glm::vec3 Object3D::getPosition() {
	return this->position;
}

glm::vec4 Object3D::getRotation() {
	return this->rotation;
}

float Object3D::getRotationInDegree() {
	return this->rotation.x;
}

float Object3D::getRotationInRadian() {
	return glm::radians(this->rotation.x);
}

glm::vec3 Object3D::getRotationVector() {
	return glm::vec3(this->rotation.y, this->rotation.z, this->rotation.w);
}

glm::vec3 Object3D::getScale() {
	return this->scale;
}

glm::vec4 Object3D::getRGBAcolor() {
	return this->color;
}

glm::vec3 Object3D::getRGBcolor() {
	return glm::vec3(this->color.y, this->color.z, this->color.w);
}

float Object3D::getOpacity() {
	return this->color.w;
}

glm::mat4 Object3D::getWorldMatrix() {
	return glm::translate(this->position)
	* glm::rotate(this->getRotationInRadian(), this->getRotationVector())
	* glm::scale(this->scale);
}

glm::vec4 Object3D::getHitSphere() {
	return this->hitSphere;
}