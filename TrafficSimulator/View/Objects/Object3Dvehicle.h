#pragma once
#include "Object3D.h"

class Object3Dvehicle: public Object3D {
public:
	Object3Dvehicle(void);
	Object3Dvehicle(ObjectStorage* objectStorage);
	Object3Dvehicle(int id, std::string name, std::string type, std::string meshID, std::string textureID, std::string lightTexture, std::string breakTexture,
	std::string rightTexture, std::string leftTexture, std::string iconID,
	glm::vec3 initPosition, glm::vec3 initScale, glm::vec4 initRotation,
	glm::vec4 color, glm::vec4	hitSphere, glm::vec4 moveSphere, ObjectStorage* objectStorage);
	~Object3Dvehicle(void);

	void setLightTexture(int);
	void setBreakTexture(int);
	void setRightTexture(int);
	void setLeftTexture (int);

	Texture2D getLightTexture();
	Texture2D getBreakTexture();
	Texture2D getRightTexture();
	Texture2D getLeftTexture ();

	Object3Dvehicle copy();
	Object3Dvehicle copy(int renderID);
protected:
	std::string lightTexture;
	std::string breakTexture;
	std::string rightTexture;
	std::string leftTexture;
};