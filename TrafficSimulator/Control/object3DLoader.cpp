#include "object3DLoader.h"
//#include <iostream>
#include <sstream>
#include <fstream>

object3Dloader::object3Dloader(void) {
}

object3Dloader::~object3Dloader(void) {
}

void loadObjects() {

}

void object3Dloader::readCSV() {
    std::ifstream file(configFile);
    std::string line;
    
    while (std::getline(file, line)) {
        std::stringstream lineStream(line);
        std::string cell;
        std::vector<std::string> parsedRow;
        while (std::getline(lineStream, cell, ';')) {
            parsedRow.push_back(cell);
        }
        parsedCSV.push_back(parsedRow);
    }
	file.close();
}
