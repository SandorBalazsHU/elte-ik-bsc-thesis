#pragma once

class Render;

#include <string>
#include <vector>

class MapLoader {
public:
	MapLoader(void);
	~MapLoader(void);
	void bind(Render* render);
	void loadMap(std::string fileName);
private:
	std::string saveFolder = "Saves\\";
	Render* render;
	std::vector<std::vector<std::string>> parsedCSV;
	void readCSV(std::string fileName);
};