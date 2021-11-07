#include <iostream>
#include <fstream>

#include "MapSaver.h"
#include "../View/Render.h"

MapSaver::MapSaver(void) {

}

MapSaver::~MapSaver(void) {

}

void MapSaver::bind(Render* render) {
	this->render = render;
}

void MapSaver::saveMap(std::string fileName) {
	std::ofstream saveFile;
	saveFile.open(this->saveFolder + fileName);
	saveFile << "Writing this to a file.\n";
	saveFile.close();
}