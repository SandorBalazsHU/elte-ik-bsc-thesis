//SU-30SM
//Never Fade Away
#include <stdlib.h>
#include <time.h>
#include "Animator.h"
#include "Render.h"
#include "WorkWindow.h"
#include "../Model/Vehicle.h"
#include "../Model/Point.h"

Animator::Animator(void) {

}

Animator::~Animator(void) {

}

int Animator::updateFrequency = 20;

int Animator::vehicleStarterUpdateFrequency = 1000;

void Animator::bind(Render* render) {
	this->render = render;
}

void Animator::update() {
	if (this->isAnimationRunning && vehicleStarterTimer()) {
		autoAdder();
	}
	if ((this->isAnimationRunning || this->softRunning) && animationTimer()) {
		for (size_t i = 0; i < vehicles.size(); i++) {
			if (!vehicles[i].isDeleted()) {
				vehicles[i].update();
			}
		}
	}
	deleteFinishedVehicles();
}

bool Animator::animationTimer() {
	bool isTime = false;
	currentTime = SDL_GetTicks();
	if (currentTime - prevouseTime >= updateFrequency) {
		prevouseTime = SDL_GetTicks();
		isTime = true;
	}
	return isTime;
}

bool Animator::vehicleStarterTimer() {
	bool isTime = false;
	vehicleStarterCurrentTime = SDL_GetTicks();
	if (vehicleStarterCurrentTime - vehicleStarterPrevouseTime >= vehicleStarterUpdateFrequency) {
		vehicleStarterPrevouseTime = SDL_GetTicks();
		isTime = true;
	}
	return isTime;
}

void Animator::finalize() {
	delete this->graph;
	startedVehiclesIndex = 0;
	this->graph = new Graph(this->render);
	this->graph->generateGraph();

	this->startPoints = graph->getStartPoints();
	this->endPoints = graph->getEndPoints();

	for (size_t i = 0; i < this->startPoints.size(); i++) {
		Point* point = this->graph->getPointByID(this->startPoints[i]);
		std::map<int, Object3Dvehicle>::iterator it = render->getWorkingWindow()->getObjectStorage()->object3Dvehicles.begin();
		while (it != render->getWorkingWindow()->getObjectStorage()->object3Dvehicles.end()) {
			if (it->second.getType() == "vehicle") {
				point->startConfiguration.push_back(it->first);
			}
			it++;
		}
		for (size_t j = 0; j < this->endPoints.size(); j++) {
			point->endpointsList.push_back(true);
		}
	}
}

void Animator::start() {
	this->isAnimationRunning = true;
}

void Animator::pause() {
	this->isAnimationRunning = false;
	this->softRunning = false;
}

void Animator::stop() {
	this->isAnimationRunning = false;
	this->softRunning = true;
	startedVehiclesIndex = 0;
	render->clearVehicles();
	this->vehicles.clear();
}

//TODO Mi van ha nincs kiválasztva autó vagy uticél
void Animator::addVehicle(size_t startPoint, bool singleVehicle) {
	if (render->isEditorLoced()) {
			Point* point = graph->getPointByID(startPoint);

			std::vector<size_t> activeVehicles = point->activeVehicles();
			size_t newVehicleID = render->addVehicle(activeVehicles[getRandomNumber(0, activeVehicles.size() - 1)], this->vehicles.size());
			size_t id = this->vehicles.size();

			std::vector<size_t> activeEndpoints = point->activeEndpoints();
			size_t endPoint = this->endPoints[activeEndpoints[getRandomNumber(0, activeEndpoints.size() - 1)]];
		
		this->vehicles.push_back(Vehicle(this->graph, this->render, startPoint, endPoint, newVehicleID, id));
		this->vehicles[this->vehicles.size() - 1].update();
		if(singleVehicle) this->softRunning = true;
	}
}

int Animator::getRandomNumber(int min, int max) {
	srand(time(NULL));
	if(min < max){
		return rand() % (max - min + 1) + min;
	} else {
		return min;
	}
	
}
//--------------------------------------------------------------DELAY-----------------------------------------------------------------
void Animator::autoAdder() {
	Point* point = graph->getPointByID(startPoints[startedVehiclesIndex]);
	if (point->startableVehicles > 0) {
		addVehicle(startPoints[startedVehiclesIndex], false);
		point->startableVehicles -= 1;
	}
	startedVehiclesIndex++;
	if (startedVehiclesIndex == this->startPoints.size()) startedVehiclesIndex = 0;
}

Graph* Animator::getGraph() {
	return this->graph;
}

void Animator::deleteFinishedVehicles() {
	for (size_t i = 0; i < this->vehicles.size(); i++) {
		if (vehicles[i].isFinished() && !vehicles[i].isDeleted()) {
			this->vehicles[i].erase();
			render->deleteVehicle(this->vehicles[i].getObject3DiD());
		}
	}
}

void Animator::clear() {
	this->vehicles.clear();
}

Vehicle* Animator::getVehicleModel(size_t modelID) {
	return &this->vehicles[modelID];
}

size_t Animator::getVehicleModelSize() {
	return this->vehicles.size();
}