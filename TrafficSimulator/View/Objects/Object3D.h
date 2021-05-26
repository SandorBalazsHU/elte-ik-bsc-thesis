#pragma once
#include <glm/glm.hpp>
#include <string>
#include "../Utilities/TextureObject.h"
#include "../Utilities/Mesh_OGL3.h"

class ObjectStorage;

class Object3D {
	public:
		Object3D() {}
		Object3D(int id, std::string name, std::string type, std::string meshID, std::string textureID, std::string iconID,
			glm::vec3 initPosition, glm::vec3 initScale, glm::vec4 initRotation,
			glm::vec4 color, glm::vec4	hitSphere, glm::vec4 moveSphere, ObjectStorage* objectStorage);
		~Object3D(void) {}
		void					setMeshID(std::string meshID);
		void					setTextureID(std::string textureID);
		void					setIconID(std::string iconID);
		void					setPosition(glm::vec3 position);
		void					setRotation(glm::vec4 rotation);
		void					setScale(glm::vec3 scale);
		void					move(glm::vec3 move);
		void					rotateLeft(float rotateLeft);
		void					rotateRight(float rotateRight);
		void					setRGBAcolor(glm::vec4 RGBAcolor);
		void					setRGBcolor(glm::vec3 RGBcolor);
		void					setOpacity(float opacity);
		std::unique_ptr<Mesh>&	getMesh();
		Texture2D&				getTexture();
		std::string				getTextureID();
		Texture2D&				getIcon();
		std::string				getIconID();
		glm::vec3				getPosition();
		glm::vec4				getRotation();
		float					getRotationInDegree();
		float					getRotationInRadian();
		glm::vec3				getRotationVector();
		glm::vec3				getScale();
		glm::vec4				getRGBAcolor();
		glm::vec3				getRGBcolor();
		float					getOpacity();
		glm::mat4				getWorldMatrix();
		glm::vec4				getHitSphere();
		glm::vec4				getMoveSphere();
		Object3D				copy();

	protected:
		ObjectStorage*			objectStorage;
		int 					id;
		std::string				name;
		std::string				type;
		std::string				meshID;
		std::string				textureID;
		std::string				iconID;
		glm::vec4				color;
		glm::vec3				position;
		glm::vec3				scale;
		glm::vec4				rotation;
		glm::vec4				worldMatrix;
		glm::vec4				hitSphere;
		glm::vec4				moveSphere;
	};