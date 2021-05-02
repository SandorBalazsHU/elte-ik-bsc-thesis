#pragma once

#include "Utilities/ProgramObject.h"
#include "Utilities/TextureObject.h"
#include "Utilities/Mesh_OGL3.h"
#include "Utilities/VertexArrayObject.h"

#include <vector>

class Object3D {
	public:
		Object3D(std::string name, int type, int meshID, int textureID, int iconID,
			glm::vec3 initPosition, glm::vec3 initScale, glm::vec4 initRotation,
			glm::vec4 color, glm::vec4	hitSphere);
		~Object3D(void);
		void		setMeshID(int);
		void		setTextureID(int);
		int			setIconID(int);
		void		setPosition(glm::vec3);
		void		setRotation(glm::vec4);
		void		setScale(glm::vec3);
		void		move(glm::vec3);
		void		rotateLeft(float);
		void		rotateRight(float);
		void		setRGBAcolor(glm::vec4);
		void		setRGBcolor(glm::vec3);
		void		setOpacity(float);
		std::unique_ptr<Mesh> getMesh();
		Texture2D	getTexture();
		int			getTextureID();
		Texture2D	getIcon();
		int			getIconID();
		glm::vec3	getPosition();
		glm::vec4	getRotation();
		glm::vec3	getScale();
		glm::vec4	getRGBAcolor();
		glm::vec3	getRGBcolor();
		float		getOpacity();
		glm::vec4	getWorldMatrix();
		glm::vec4	getHitSphere();

	protected:
		int	meshID;
		int	textureID;
		int iconID;
		static std::vector<std::unique_ptr<Mesh>>	meshStorage;
		static std::vector<VertexArrayObject>		generatedMeshStorage;
		static std::vector<Texture2D>				tetureStorage;
		static std::vector<Texture2D>				iconStorage;
		glm::vec4	color;
		glm::vec3	initPosition;
		glm::vec3	initScale;
		glm::vec4	initRotation;
		glm::vec3	position;
		glm::vec3	scale;
		glm::vec4	rotation;
		glm::vec4	worldMatrix;
		glm::vec4	hitSphere;
	};