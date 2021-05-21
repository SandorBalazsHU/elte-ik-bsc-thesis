#pragma once

#include <GL\glew.h>
#include <GL\GL.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string>

enum class TextureType
{
	Texture1D					= GL_TEXTURE_1D, 
	Texture2D					= GL_TEXTURE_2D, 
	Texture3D					= GL_TEXTURE_3D, 
	Texture1DArray				= GL_TEXTURE_1D_ARRAY, 
	Texture2DArray				= GL_TEXTURE_2D_ARRAY, 
	TextureRectangle			= GL_TEXTURE_RECTANGLE, 
	TextureCubeMap				= GL_TEXTURE_CUBE_MAP, 
	TextureCubeMapArray			= GL_TEXTURE_CUBE_MAP_ARRAY, 
	Texture2DMultisample		= GL_TEXTURE_2D_MULTISAMPLE,
	Texture2DMultisampleArray	= GL_TEXTURE_2D_MULTISAMPLE_ARRAY
};

template <TextureType type = TextureType::Texture2D>
class TextureObject final
{
public:
	TextureObject();
	TextureObject(const std::string&);
	~TextureObject();

	TextureObject(const TextureObject&) = delete;
	TextureObject& operator=(const TextureObject&) = delete;
	
	TextureObject(TextureObject&& rhs);
	TextureObject& operator=(TextureObject&& rhs);

	TextureObject& operator=(const std::string& s);

	void AttachFromFile(const std::string&, bool generateMipMap = true, GLuint role = static_cast<GLuint>(type));
	void FromFileParallel(const std::string&);
	void FromFile(const std::string&);
	void bindTexture();

	operator unsigned int() const { return m_id; }

	void Clean();

private:
	GLuint m_id{};
	SDL_Surface* loaded_img;
	SDL_Surface* formattedSurf;
};

#include "TextureObject.inl"

using Texture2D = TextureObject<>;