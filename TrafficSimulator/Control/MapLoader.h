#pragma once

class Render;

#include <string>

class MapLoader {
public:
	MapLoader(void);
	~MapLoader(void);
	void bind(Render* render);
	void saveMap(std::string fileName);
private:
	Render* render;
};