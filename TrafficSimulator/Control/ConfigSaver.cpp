#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <filesystem>
#include "ConfigSaver.h"
#include "Logger.h"


void ConfigSaver::setProperty(std::string propertyName, std::string value) {
	readCSV(this->configFile);

	for (size_t i = 1; i < parsedCSV.size(); i++) {
		std::string conifName = parsedCSV[i][0];
		if (conifName == propertyName) {
			parsedCSV[i][1] = value;
		}
	}

	writeCSV(this->configFile);
}

void ConfigSaver::writeCSV(std::string fileName) {
	std::ofstream saveFile;
	saveFile.open(fileName);
	for (size_t i = 0; i < parsedCSV.size(); i++) {
		for (size_t j = 0; j < parsedCSV[i].size(); j++) {
			if (j < parsedCSV[i].size() - 1) {
				saveFile << parsedCSV[i][j] << ";";
			} else {
				saveFile << parsedCSV[i][j];
			}
		}
		saveFile << std::endl;
	}
	saveFile.close();
}

void ConfigSaver::readCSV(std::string fileName) {
	parsedCSV.clear();
	std::ifstream file(fileName);
	std::string line;
	int lineCount = 0;
	while (std::getline(file, line)) {
		std::stringstream lineStream(line);
		std::string cell;
		std::vector<std::string> parsedRow;
		while (std::getline(lineStream, cell, ';')) {
			parsedRow.push_back(cell);
		}
		parsedCSV.push_back(parsedRow);
		lineCount++;
	}
	file.close();
}
