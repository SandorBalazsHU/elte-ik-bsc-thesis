#pragma once
#include <vector>;
#include "../View/Utilities/ProgramObject.h"
#include "../View/Utilities/TextureObject.h"
#include "../View/Utilities/Mesh_OGL3.h"

class object3Dloader {
public:
	object3Dloader(void);
	~object3Dloader(void);

private:
	const std::string configFile = "3Dobjects/3d_object_library.csv";
	std::vector<std::vector<std::string>> parsedCSV;
	void readCSV();
};