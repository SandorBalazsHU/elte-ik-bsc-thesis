#pragma once
#include <glm/glm.hpp>
#include <string>
#include "../Utilities/TextureObject.h"
#include "../Utilities/Mesh_OGL3.h"

class ObjectStorage;

class Object3D {
	public:
		Object3D(ObjectStorage* objectStorage);
		Object3D(int id, std::string name, std::string type, std::string meshID, std::string textureID, std::string iconID,
			glm::vec3 initPosition, glm::vec3 initScale, glm::vec4 initRotation,
			glm::vec4 color, glm::vec4	hitSphere, glm::vec4 moveSphere, ObjectStorage* objectStorage);
		~Object3D(void);
		void					setDependencyID(int dependencyID);
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
		void 					setRenderID(int newRenderID);
		void 					setProtection(bool isProtected);
		void 					setSelectable(bool isSelectable);
		int 					getId();
		int 					getRenderID();
		int						getDependencyID();
		std::string				getName();
		std::string				getType();
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
		bool					isProtected();
		bool					isSelectable();
		Object3D				copy();
		Object3D				copy(int renderID);
		bool					isHidden();
		void					hide();
		void					show();
		void					erase();
		bool					isDeleted();
		size_t					getModelID();
		void					setModelID(size_t id);
		void					select();
		void					deSelect();
		bool					isSelected();

	protected:
		ObjectStorage*			objectStorage;
		int 					id = -1;
		int 					renderID = -1;
		int						dependencyID = -1;
		std::string				name = "";
		std::string				type = "";
		std::string				meshID = "";
		std::string				textureID = "white.png";
		std::string				iconID = "sphere_mark_mini.png";
		glm::vec4				color = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3				position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3				scale = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec4				rotation = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
		glm::vec4				worldMatrix = glm::vec4(1.0f);
		glm::vec4				hitSphere = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		glm::vec4				moveSphere = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		bool					hidden = false;
		bool					protection = false;
		bool					selectable = true;
		bool					deleted = false;
		size_t					modelID = -1;
		bool					selected = true;
	};