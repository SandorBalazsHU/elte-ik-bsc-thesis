#pragma once

class Render;

#include <string>

class MapSaver {
public:
	MapSaver(void);
	~MapSaver(void);
	void bind(Render* render);
	void saveMap(std::string fileName);
private:
	std::string saveFolder = "Saves\\";
	Render* render;
};