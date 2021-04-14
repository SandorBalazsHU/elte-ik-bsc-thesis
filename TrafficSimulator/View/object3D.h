#pragma once

#include "Utilities/ProgramObject.h"
#include "Utilities/TextureObject.h"
#include "Utilities/Mesh_OGL3.h"

class object3D {
	public:
		object3D(void);
		~object3D(void);
		std::unique_ptr<Mesh> getModel();
		void		setModel(std::unique_ptr<Mesh>);
		void		setTexture(Texture2D);
		void		setPosition(glm::vec3);
		void		setRotation(glm::vec4);
		void		setScale(glm::vec3);
		void		move(glm::vec3);
		void		rotateLeft(float);
		void		rotateRight(float);
		void		setRGBAcolor(glm::vec4);
		void		setRGBcolor(glm::vec3);
		void		setOpacity(float);
		Texture2D	getTexture();
		glm::vec3	getPosition();
		glm::vec4	getRotation();
		glm::vec3	getScale();
		glm::vec4	getRGBAcolor();
		glm::vec3	getRGBcolor();
		float		getOpacity();
		glm::vec4	getHitSphere();

	protected:
		std::unique_ptr<Mesh>	modell;
		glm::mat4	transformation;
		Texture2D	texture;
		glm::vec4	color;
		glm::vec3	initPosition;
		glm::vec3	initScale;
		glm::vec4	initRotation;
		glm::vec3	position;
		glm::vec3	scale;
		glm::vec4	rotation;
		glm::vec4	hitSphere;
	};