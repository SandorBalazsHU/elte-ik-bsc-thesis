#pragma once

class Render;

#include <string>
#include <vector>

class MapLoader {
public:
	/*MapLoader(void);
	~MapLoader(void);*/
	void bind(Render* render);
	void loadMap(std::string fileName);
	std::vector<std::string> listFiles();
	int deleteSave(std::string fileName);
private:
	const std::string fileType = ".csv";
	std::string saveFolder = "Saves\\";
	Render* render = NULL;
	std::vector<std::vector<std::string>> parsedCSV;
	void readCSV(std::string fileName);
};