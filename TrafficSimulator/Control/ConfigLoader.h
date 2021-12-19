#pragma once

//Understanding is a three edged sword.

class Render;

#include <string>
#include <vector>

class ConfigLoader {
public:
	void bind(Render* render);
	void loadConfig();
private:
	std::string configFile = "config.csv";
	Render* render = NULL;
	std::vector<std::vector<std::string>> parsedCSV;
	void readCSV(std::string fileName);
};