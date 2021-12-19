#pragma once

//Alarm 1202, Alarm 1201

#include <string>
#include <vector>

class ConfigSaver {
public:
	void setProperty(std::string propertyName, std::string value);

private:
	std::string configFile = "config.csv";
	std::vector<std::vector<std::string>> parsedCSV;
	void writeCSV(std::string fileName);
	void readCSV(std::string fileName);
};