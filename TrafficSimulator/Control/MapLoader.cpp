#include "MapLoader.h"
#include "../View/Render.h"

MapLoader::MapLoader(void) {

}

MapLoader::~MapLoader(void) {

}

void MapLoader::bind(Render* render) {
	this->render = render;
}

void MapLoader::saveMap(std::string fileName) {

}