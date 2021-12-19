/**
 * @name Traffic Simulation
 * @file Object3D.h
 * @class Object3D
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief The Object3D renderable object representation.
 * Contact: sandorbalazs9402@gmail.com
*/

#pragma once

#include <glm/glm.hpp>
#include <string>
#include "../Utilities/TextureObject.h"
#include "../Utilities/Mesh_OGL3.h"

/**
 * @brief Circular include prevention.
*/
class ObjectStorage;

/**
 * @brief The 3D renderable object representation.
*/
class Object3D {
	public:

		/**
		 * @brief Empty constructor for the ObjectStorage.
		 * @param objectStorage The copyable ObjectStorage.
		*/
		Object3D(ObjectStorage* objectStorage);

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
		Object3D(int id, std::string name, std::string type, std::string meshID, std::string textureID, std::string iconID,
			glm::vec3 initPosition, glm::vec3 initScale, glm::vec4 initRotation,
			glm::vec4 color, glm::vec4	hitSphere, glm::vec4 moveSphere, ObjectStorage* objectStorage);

		/**
		 * @brief The Object3D destructor.
		 * @param objectStorage
		*/
		~Object3D(void);

		/**
		 * @brief Setter for the dependencyID. (Used when the object has dependency.(road))
		 * @param dependencyID The dependencyID
		*/
		void					setDependencyID(int dependencyID);

		/**
		 * @brief Setter for the mesh.
		 * @param meshID The mesh.
		*/
		void					setMeshID(std::string meshID);

		/**
		 * @brief Setter for the Texture ID.
		 * @param textureID the Texture ID,
		*/
		void					setTextureID(std::string textureID);

		/**
		 * @brief Setter for the Icon ID.
		 * @param iconID The Icon ID.
		*/
		void					setIconID(std::string iconID);

		/**
		 * @brief Setter for the position.
		 * @param position The new position.
		*/
		void					setPosition(glm::vec3 position);

		/**
		 * @brief Setter for the new rotation.
		 * @param rotation The new rotation.
		*/
		void					setRotation(glm::vec4 rotation);

		/**
		 * @brief Setter for the new scale.
		 * @param scale The new scale.
		*/
		void					setScale(glm::vec3 scale);

		/**
		 * @brief Shift the position.
		 * @param move The shift vector.
		*/
		void					move(glm::vec3 move);

		/**
		 * @brief Rotate the rotation left.
		 * @param rotateLeft The left rotation.
		*/
		void					rotateLeft(float rotateLeft);

		/**
		 * @brief Rotate the rotation right.
		 * @param rotateLeft The left right.
		*/
		void					rotateRight(float rotateRight);

		/**
		 * @brief Setter for the RGBA color.
		 * @param RGBAcolor The new RGBA color.
		*/
		void					setRGBAcolor(glm::vec4 RGBAcolor);

		/**
		 * @brief Setter for the RGB color.
		 * @param RGBAcolor The new RGB color.
		*/
		void					setRGBcolor(glm::vec3 RGBcolor);

		/**
		 * @brief Setter for the opacity.
		 * @param opacity The new opacity.
		*/
		void					setOpacity(float opacity);

		/**
		 * @brief The object current renderID from the render.
		 * @param newRenderID The new render ID.
		*/
		void 					setRenderID(int newRenderID);

		/**
		 * @brief Set the object protection.
		 * @param isProtected The new protection state.
		*/
		void 					setProtection(bool isProtected);

		/**
		 * @brief Set the selectable flag.
		 * @param isSelectable The new state for the selectable flag.
		*/
		void 					setSelectable(bool isSelectable);

		/**
		 * @brief Getter for the ID.
		 * @return The ID.
		*/
		int 					getId();

		/**
		 * @brief Getter for the renderID.
		 * @return The renderID.
		*/
		int 					getRenderID();

		/**
		 * @brief Getter for the dependency ID.
		 * @return The dependency ID.
		*/
		int						getDependencyID();

		/**
		 * @brief Getter for the name.
		 * @return The name.
		*/
		std::string				getName();

		/**
		 * @brief Getter for the Type.
		 * @return The type.
		*/
		std::string				getType();

		/**
		 * @brief Getter for the Mesh.
		 * @return The mesh.
		*/
		std::unique_ptr<Mesh>&	getMesh();

		/**
		 * @brief Getter for the texture.
		 * @return The texture.
		*/
		Texture2D&				getTexture();

		/**
		 * @brief Getter for the texture ID.
		 * @return The texture ID.
		*/
		std::string				getTextureID();

		/**
		 * @brief Getter for the Icon.
		 * @return The icon.
		*/
		Texture2D&				getIcon();

		/**
		 * @brief Getter for the icon ID.
		 * @return The icon ID.
		*/
		std::string				getIconID();

		/**
		 * @brief Getter for the position.
		 * @return The Position.
		*/
		glm::vec3				getPosition();

		/**
		 * @brief Getter for the rotation.
		 * @return The rotation.
		*/
		glm::vec4				getRotation();

		/**
		 * @brief Getter for the rotation in degree.
		 * @return The rotation in degree.
		*/
		float					getRotationInDegree();

		/**
		 * @brief Getter for the rotation in radian.
		 * @return the rotation in radian.
		*/
		float					getRotationInRadian();

		/**
		 * @brief Getter for the rotation vector.
		 * @return The rotation vector.
		*/
		glm::vec3				getRotationVector();

		/**
		 * @brief Getter for the scale.
		 * @return The scale.
		*/
		glm::vec3				getScale();

		/**
		 * @brief Getter for the RGBA color.
		 * @return The RGBA color.
		*/
		glm::vec4				getRGBAcolor();

		/**
		 * @brief Getter for the RGB color.
		 * @return RGB color.
		*/
		glm::vec3				getRGBcolor();

		/**
		 * @brief Getter for the opacity.
		 * @return The opacity.
		*/
		float					getOpacity();

		/**
		 * @brief Calculate the world matrix.
		 * @return The calculated World Matrix.
		*/
		glm::mat4				getWorldMatrix();

		/**
		 * @brief Getter for the hit sphere.
		 * @return The hit sphere.
		*/
		glm::vec4				getHitSphere();

		/**
		 * @brief Getter for the move sphere.
		 * @return The move sphere.
		*/
		glm::vec4				getMoveSphere();

		/**
		 * @brief Getter for the protection flag.
		 * @return The protection flag.
		*/
		bool					isProtected();

		/**
		 * @brief Getter for the selectable flag.
		 * @return The selectable flag.
		*/
		bool					isSelectable();

		/**
		 * @brief Object copy method.
		 * @return The copy of this object.
		*/
		Object3D				copy();

		/**
		 * @brief Object copy method. (And set render id too.)
		 * @param renderID The settable render ID.
		 * @return The copy of this object.
		*/
		Object3D				copy(int renderID);

		/**
		 * @brief Getter for the hidden flag.
		 * @return The hidden flag.
		*/
		bool					isHidden();

		/**
		 * @brief Hide the object.
		*/
		void					hide();

		/**
		 * @brief Show the object.
		*/
		void					show();

		/**
		 * @brief Set the object deleted.
		*/
		void					erase();

		/**
		 * @brief Getter for the deleted flag.
		 * @return The deleted flag.
		*/
		bool					isDeleted();

		/**
		 * @brief Getter for the Model ID. (Link for the graph model.)
		 * @return The Model ID.
		*/
		size_t					getModelID();

		/**
		 * @brief Setter for the model ID. (Link for the graph model.)
		 * @param id The new model ID.
		*/
		void					setModelID(size_t id);

		/**
		 * @brief Set the object selected.
		*/
		void					select();

		/**
		 * @brief Set the object deselect.
		*/
		void					deSelect();

		/**
		 * @brief Getter for the selected flag.
		 * @return The selected flag.
		*/
		bool					isSelected();

	protected:

		/**
		 * @brief Pointer for the objects storage.
		*/
		ObjectStorage*			objectStorage;

		/**
		 * @brief The object type ID.
		*/
		int 					id = -1;

		/**
		 * @brief The current object render ID in the render.
		*/
		int 					renderID = -1;

		/**
		 * @brief The dependency id if it exist.
		*/
		int						dependencyID = -1;

		/**
		 * @brief The object type name.
		*/
		std::string				name = "";

		/**
		 * @brief The object type.
		*/
		std::string				type = "";

		/**
		 * @brief The geometry ID.
		*/
		std::string				meshID = "";

		/**
		 * @brief The texture ID.
		*/
		std::string				textureID = "white.png";

		/**
		 * @brief The Icon ID.
		*/
		std::string				iconID = "sphere_mark_mini.png";

		/**
		 * @brief The color.
		*/
		glm::vec4				color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		/**
		 * @brief The default position.
		*/
		glm::vec3				defaultPosition = glm::vec3(0.0f, 0.0f, 0.0f);

		/**
		 * @brief The current position.
		*/
		glm::vec3				position = glm::vec3(0.0f, 0.0f, 0.0f);

		/**
		 * @brief The current scale.
		*/
		glm::vec3				scale = glm::vec3(1.0f, 1.0f, 1.0f);

		/**
		 * @brief The current rotation.
		*/
		glm::vec4				rotation = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);

		/**
		 * @brief The current calculated world matrix.
		*/
		glm::vec4				worldMatrix = glm::vec4(1.0f);

		/**
		 * @brief The hit sphere for the ray picking.
		*/
		glm::vec4				hitSphere = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

		/**
		 * @brief The move sphere.
		*/
		glm::vec4				moveSphere = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		/**
		 * @brief The hidden flag.
		*/
		bool					hidden = false;

		/**
		 * @brief The protection flag.
		*/
		bool					protection = false;

		/**
		 * @brief The selectable flag.
		*/
		bool					selectable = true;

		/**
		 * @brief The deleted flag.
		*/
		bool					deleted = false;

		/**
		 * @brief The graph model id.
		*/
		size_t					modelID = -1;

		/**
		 * @brief The selected flag.
		*/
		bool					selected = true;
	};