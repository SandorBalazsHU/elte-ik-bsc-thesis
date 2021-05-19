#pragma once
#include <vector>
#include <map>
#include "../View/Utilities/ProgramObject.h"
#include "../View/Utilities/TextureObject.h"
#include "../View/Utilities/Mesh_OGL3.h"

class object3Dloader {
public:
	object3Dloader(void);
	~object3Dloader(void);
	void load();
	std::map<std::string, Texture2D> textures;
private:
	const std::string configFile = "3Dobjects/3d_object_library.csv";
	const std::string textureFolder = "3Dobjects/textures/";
	std::vector<std::vector<std::string>> parsedCSV;
	void readCSV();
};