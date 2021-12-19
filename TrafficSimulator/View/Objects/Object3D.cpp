#include "Object3D.h"
#include "ObjectStorage.h"
#include <glm/gtx/transform2.hpp>
#include <iostream>

/**
 * @brief Empty constructor for the ObjectStorage.
 * @param objectStorage The copyable ObjectStorage.
*/
Object3D::Object3D(ObjectStorage* objectStorage) {
	this->objectStorage = objectStorage;
}

/**
 * @brief The full constructor for the Object3D class.
 * @param id The object type ID.
 * @param name The object type name.
 * @param type The object category.
 * @param meshID The geometry.
 * @param textureID The Texture.
 * @param iconID The icon texture.
 * @param initPosition The initialization position.
 * @param initScale The initialization scale.
 * @param initRotation The initialization rotation.
 * @param color The base color.
 * @param hitSphere The hit sphere for the ray picking.
 * @param moveSphere The move sphere.
 * @param objectStorage The pointer for the object storage.
*/
Object3D::Object3D(int id, std::string name, std::string type, std::string meshID, std::string textureID, std::string iconID,
	glm::vec3 initPosition, glm::vec3 initScale, glm::vec4 initRotation,
	glm::vec4 color, glm::vec4 hitSphere, glm::vec4 moveSphere, ObjectStorage* objectStorage) {
	this->id = id;
	this->renderID = -1;
	this->dependencyID = -1;
	this->name = name;
	this->type = type;
	this->meshID = meshID;
	this->textureID = textureID;
	this->iconID = iconID;
	this->defaultPosition = initPosition;
	this->position = initPosition;
	this->scale = initScale;
	this->rotation = initRotation;
	this->color = color;
	this->hitSphere = hitSphere;
	this->moveSphere = moveSphere;
	this->objectStorage = objectStorage;
}

/**
 * @brief The Object3D destructor.
 * @param objectStorage
*/
Object3D::~Object3D(void) {};

/**
 * @brief The object current renderID from the render.
 * @param newRenderID The new render ID.
*/
void Object3D::setRenderID(int newRenderID) {
	this->renderID = newRenderID;
}

/**
 * @brief Setter for the dependencyID. (Used when the object has dependency.(road))
 * @param dependencyID The dependencyID
*/
void Object3D::setDependencyID(int dependencyID) {
	this->dependencyID = dependencyID;
}

/**
 * @brief Setter for the mesh.
 * @param meshID The mesh.
*/
void Object3D::setMeshID(std::string meshID) {
	this->meshID = meshID;
}

/**
 * @brief Setter for the Texture ID.
 * @param textureID the Texture ID,
*/
void Object3D::setTextureID(std::string textureID) {
	this->textureID = textureID;
}

/**
 * @brief Setter for the Icon ID.
 * @param iconID The Icon ID.
*/
void Object3D::setIconID(std::string iconID) {
	this->iconID = iconID;
}

/**
 * @brief Setter for the position.
 * @param position The new position.
*/
void Object3D::setPosition(glm::vec3 position) {
	this->position = position;
}

/**
 * @brief Setter for the new rotation.
 * @param rotation The new rotation.
*/
void Object3D::setRotation(glm::vec4 rotation) {
	this->rotation = rotation;
}

/**
 * @brief Setter for the new scale.
 * @param scale The new scale.
*/
void Object3D::setScale(glm::vec3 scale) {
	this->scale = scale;
}

/**
 * @brief Shift the position.
 * @param move The shift vector.
*/
void Object3D::move(glm::vec3 move) {
		this->position += move;
}

/**
 * @brief Rotate the rotation left.
 * @param rotateLeft The left rotation.
*/
void Object3D::rotateLeft(float rotateLeft) {
	this->rotation += glm::vec4(rotateLeft, 0, 0, 0);
}

/**
 * @brief Rotate the rotation right.
 * @param rotateLeft The left right.
*/
void Object3D::rotateRight(float rotateRight) {
	this->rotation += glm::vec4(-rotateRight, 0, 0, 0);
}

/**
 * @brief Setter for the RGBA color.
 * @param RGBAcolor The new RGBA color.
*/
void Object3D::setRGBAcolor(glm::vec4 RGBAcolor) {
	this->color = RGBAcolor;
}

/**
 * @brief Setter for the RGB color.
 * @param RGBAcolor The new RGB color.
*/
void Object3D::setRGBcolor(glm::vec3 RGBcolor) {
	this->color.x = RGBcolor.x;
	this->color.y = RGBcolor.y;
	this->color.z = RGBcolor.z;
}

/**
 * @brief Setter for the opacity.
 * @param opacity The new opacity.
*/
void Object3D::setOpacity(float opacity) {
	this->color.w = opacity;
}

/**
 * @brief Set the object protection.
 * @param isProtected The new protection state.
*/
void Object3D::setProtection(bool isProtected) {
	this->protection = isProtected;
}

/**
 * @brief Set the selectable flag.
 * @param isSelectable The new state for the selectable flag.
*/
void Object3D::setSelectable(bool isSelectable) {
	this->selectable = isSelectable;
}

/**
 * @brief Getter for the ID.
 * @return The ID.
*/
int	Object3D::getId() {
	return this->id;
}

/**
 * @brief Getter for the renderID.
 * @return The renderID.
*/
int	Object3D::getRenderID() {
	return this->renderID;
}

/**
 * @brief Getter for the dependency ID.
 * @return The dependency ID.
*/
int	Object3D::getDependencyID() {
	return this->dependencyID;
}

/**
 * @brief Getter for the name.
 * @return The name.
*/
std::string	Object3D::getName() {
	return this->name;
}

/**
 * @brief Getter for the Type.
 * @return The type.
*/
std::string	Object3D::getType() {
	return this->type;
}

/**
 * @brief Getter for the Mesh.
 * @return The mesh.
*/
std::unique_ptr<Mesh>& Object3D::getMesh() {
	return objectStorage->getMesh(this->meshID);
}

/**
 * @brief Getter for the texture.
 * @return The texture.
*/
Texture2D& Object3D::getTexture() {
	return objectStorage->getTexture(this->textureID);
}

/**
 * @brief Getter for the texture ID.
 * @return The texture ID.
*/
std::string Object3D::getTextureID() {
	return this->textureID;
}

/**
 * @brief Getter for the Icon.
 * @return The icon.
*/
Texture2D& Object3D::getIcon() {
	return objectStorage->getTexture(this->iconID);
}

/**
 * @brief Getter for the icon ID.
 * @return The icon ID.
*/
std::string Object3D::getIconID() {
	return this->iconID;
}

/**
 * @brief Getter for the position.
 * @return The Position.
*/
glm::vec3 Object3D::getPosition() {
	return this->position;
}

/**
 * @brief Getter for the rotation.
 * @return The rotation.
*/
glm::vec4 Object3D::getRotation() {
	return this->rotation;
}

/**
 * @brief Getter for the rotation in degree.
 * @return The rotation in degree.
*/
float Object3D::getRotationInDegree() {
	return this->rotation.x;
}

/**
 * @brief Getter for the rotation in radian.
 * @return the rotation in radian.
*/
float Object3D::getRotationInRadian() {
	return glm::radians(this->rotation.x);
}

/**
 * @brief Getter for the rotation vector.
 * @return The rotation vector.
*/
glm::vec3 Object3D::getRotationVector() {
	return glm::vec3(this->rotation.y, this->rotation.z, this->rotation.w);
}

/**
 * @brief Getter for the scale.
 * @return The scale.
*/
glm::vec3 Object3D::getScale() {
	return this->scale;
}

/**
 * @brief Getter for the RGBA color.
 * @return The RGBA color.
*/
glm::vec4 Object3D::getRGBAcolor() {
	return this->color;
}

/**
 * @brief Getter for the RGB color.
 * @return RGB color.
*/
glm::vec3 Object3D::getRGBcolor() {
	return glm::vec3(this->color.y, this->color.z, this->color.w);
}

/**
 * @brief Getter for the opacity.
 * @return The opacity.
*/
float Object3D::getOpacity() {
	return this->color.w;
}

/**
 * @brief Getter for the protection flag.
 * @return The protection flag.
*/
bool Object3D::isProtected() {
	return protection;
}

/**
 * @brief Getter for the selectable flag.
 * @return The selectable flag.
*/
bool Object3D::isSelectable() {
	return selectable;
}

/**
 * @brief Calculate the world matrix.
 * @return The calculated World Matrix.
*/
glm::mat4 Object3D::getWorldMatrix() {
	return glm::translate(this->position)
	* glm::rotate(this->getRotationInRadian(), this->getRotationVector())
	* glm::scale(this->scale);
}

/**
 * @brief Getter for the hit sphere.
 * @return The hit sphere.
*/
glm::vec4 Object3D::getHitSphere() {
	glm::vec4 sphere = this->hitSphere;
	sphere.x += this->position.x;
	sphere.y += this->position.y;
	sphere.z += this->position.z;
	return sphere;
}

/**
 * @brief Getter for the move sphere.
 * @return The move sphere.
*/
glm::vec4 Object3D::getMoveSphere() {
	glm::vec4 sphere = this->moveSphere;
	sphere.x += this->position.x;
	sphere.y += this->position.y;
	sphere.z += this->position.z;
	return sphere;
}

/**
 * @brief Object copy method.
 * @return The copy of this object.
*/
Object3D Object3D::copy() {
	return Object3D(this->id, this->name, this->type, this->meshID, this->textureID, this->iconID,
		this->position, this->scale, this->rotation,
		this->color, this->hitSphere, this->moveSphere, this->objectStorage);
}

/**
 * @brief Object copy method. (And set render id too.)
 * @param renderID The settable render ID.
 * @return The copy of this object.
*/
Object3D Object3D::copy(int renderID) {
	Object3D newObject = this->copy();
	newObject.setRenderID(renderID);
	return newObject;
}

/**
 * @brief Getter for the hidden flag.
 * @return The hidden flag.
*/
bool Object3D::isHidden() {
	return hidden;
}

/**
 * @brief Hide the object.
*/
void Object3D::hide() {
	hidden = true;
}

/**
 * @brief Show the object.
*/
void Object3D::show() {
	hidden = false;
}

/**
 * @brief Set the object deleted.
*/
void Object3D::erase() {
	deleted = true;
}

/**
 * @brief Getter for the deleted flag.
 * @return The deleted flag.
*/
bool Object3D::isDeleted() {
	return deleted;
}

/**
 * @brief Getter for the Model ID. (Link for the graph model.)
 * @return The Model ID.
*/
size_t Object3D::getModelID() {
	return this->modelID;
}

/**
 * @brief Setter for the model ID. (Link for the graph model.)
 * @param id The new model ID.
*/
void Object3D::setModelID(size_t id) {
	this->modelID = id;
}

/**
 * @brief Set the object selected.
*/
void Object3D::select() {
	this->setOpacity(0.5);
	this->selected = true;
}

/**
 * @brief Set the object deselect.
*/
void Object3D::deSelect() {
	this->setOpacity(1.0);
	this->selected = false;
}

/**
 * @brief Getter for the selected flag.
 * @return The selected flag.
*/
bool Object3D::isSelected() {
	this->selected;
}