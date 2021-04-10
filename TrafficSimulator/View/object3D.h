#pragma once

#include "Utilities/ProgramObject.h"
#include "Utilities/TextureObject.h"
#include "Utilities/Mesh_OGL3.h"

class object3D {
public:
private:
	Texture2D				texture;
	std::unique_ptr<Mesh>	modell;
};