#pragma once

#include "../View/Utilities/ProgramObject.h"
#include "../View/Utilities/TextureObject.h"
#include "../View/Utilities/Mesh_OGL3.h"

class object3Dloader {
public:
	object3Dloader(void);
	~object3Dloader(void);
	void setModel(std::unique_ptr<Mesh>);
	void setTexture(Texture2D);
	void setPosition(glm::vec3);
	void setRotation(glm::vec4);
	void setScale(glm::vec3);
	void move(glm::vec3);
	void rotateLeft(float);
	void rotateRight(float);
	std::unique_ptr<Mesh> getModel();
	Texture2D getTexture();
	glm::vec3 getPosition();
	glm::vec4 getRotation();
	glm::vec3 getScale();


private:
	glm::mat4				transformation;
	Texture2D				baseTexture;
	std::unique_ptr<Mesh>	modell;
	glm::vec3				initPosition;
	glm::vec3				initScale;
	glm::vec4				initRotation;
	glm::vec3				position;
	glm::vec3				scale;
	glm::vec4				rotation;
};