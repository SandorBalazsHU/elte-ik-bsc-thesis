#pragma once

class Render;

#include <string>

class MapSaver {
public:
	MapSaver(void);
	~MapSaver(void);
	void bind(Render* render);
	void saveMap(std::string fileName);
	std::string getLastSave();
	void setLastSave(std::string loadedMap);
	std::string getLastSaveTime();
	void setLastSaveTime();
	void reset();
private:
	std::string currentSave = "Unsaved";
	const std::string saveFolder = "Saves\\";
	std::string lastSaveTime = "Unsaved";
	Render* render;
};