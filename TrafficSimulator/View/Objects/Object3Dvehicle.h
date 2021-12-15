/**
 * @name Traffic Simulation
 * @file Object3Dvehicle.cpp
 * @class Object3Dvehicle
 * @author Sándor Balázs - AZA6NL
 * @date 2021.12.15.
 * @brief The renderable vehicle representation.
 * Contact: sandorbalazs9402@gmail.com
*/

#pragma once

#include "Object3D.h"

/**
 * @brief The renderable vehicle representation. Store all special data for the vehicles.
 */
class Object3Dvehicle: public Object3D {
public:

	/**
	 * @brief Constructor for the empty Vehicle object.
	 * @param objectStorage The object storage pointer.
	*/
	Object3Dvehicle(ObjectStorage* objectStorage);

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
	Object3Dvehicle(int id, std::string name, std::string type, std::string meshID, std::string textureID, std::string lightTexture, std::string breakTexture,
	std::string rightTexture, std::string leftTexture, std::string iconID,
	glm::vec3 initPosition, glm::vec3 initScale, glm::vec4 initRotation,
	glm::vec4 color, glm::vec4	hitSphere, glm::vec4 moveSphere, ObjectStorage* objectStorage);

	/**
	 * @brief The vehicle destructor.
	 * @param  Empty.
	*/
	~Object3Dvehicle(void);

	/**
	 * @brief Setter for special Texture setter (Not implemented yet.)
	 * @param The new texture ID
	*/
	void setLightTexture(int);

	/**
	 * @brief Setter for special Texture setter (Not implemented yet.)
	 * @param The new texture ID
	*/
	void setBreakTexture(int);

	/**
	 * @brief Setter for special Texture setter (Not implemented yet.)
	 * @param The new texture ID
	*/
	void setRightTexture(int);

	/**
	 * @brief Setter for special Texture setter (Not implemented yet.)
	 * @param The new texture ID
	*/
	void setLeftTexture (int);

	/**
	 * @brief Getter for special texture. (Not implemented yet.)
	 * @return The needed special texture.
	*/
	Texture2D getLightTexture();

	/**
	 * @brief Getter for special texture. (Not implemented yet.)
	 * @return The needed special texture.
	*/
	Texture2D getBreakTexture();

	/**
	 * @brief Getter for special texture. (Not implemented yet.)
	 * @return The needed special texture.
	*/
	Texture2D getRightTexture();

	/**
	 * @brief Getter for special texture. (Not implemented yet.)
	 * @return The needed special texture.
	*/
	Texture2D getLeftTexture ();

	/**
	 * @brief Copy this object to an other and return it.
	 * @return The copy from this object.
	*/
	Object3Dvehicle copy();

	/**
	 * @brief Copy this object to an other and return it and set the render ID.
	 * @param renderID The new render id.
	 * @return The copy from this object.
	*/
	Object3Dvehicle copy(int renderID);

	/**
	 * @brief Calculate the vehicle rotation from the road current and next point.
	 * @param pointA The current point of the road.
	 * @param pointB The next point of the road.
	 * @return The calculated direction.
	*/
	static float getMoveRtation(glm::vec3 pointA, glm::vec3 pointB);

protected:
	/**
	 * @brief Light texture.
	*/
	std::string lightTexture;

	/**
	 * @brief break light texture,
	*/
	std::string breakTexture;

	/**
	 * @brief right marker texture.
	*/
	std::string rightTexture;

	/**
	 * @brief Left marker texture.
	*/
	std::string leftTexture;
};