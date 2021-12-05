//SU-30SM
#include <stdlib.h>
#include <time.h>
#include "Animator.h"
#include "Render.h"
#include "WorkWindow.h"
#include "../Model/Vehicle.h"

Animator::Animator(void) {

}

Animator::~Animator(void) {

}

void Animator::bind(Render* render) {
	this->render = render;
}

void Animator::update() {
	timerUpdate();
	if (this->isAnimationRunning) {
		for (size_t i = 0; i < vehicles.size(); i++) {
			if (!vehicles[i].isDeleted()) {
				vehicles[i].update();
			}
		}
	}
	deleteFinishedVehicles();
}

void Animator::timerUpdate() {
	currentTime = SDL_GetTicks();
	if (currentTime - prevouseTime >= 20 && this->isAnimationRunning) {
		if (currentIndex < 100 - 2 && this->isAnimationRunning) {
			currentIndex++;
			//std::cout << currentIndex << std::endl;
			prevouseTime = SDL_GetTicks();
		}
	}
}

void Animator::finalize() {
	delete this->graph;
	this->graph = new Graph(this->render);
	this->graph->generateGraph();
	for (size_t i = 0; i < graph->getPointsNumber(); i++) {
		if (graph->getPoint(i)->isStartPoint()) {
			std::map<int, Object3Dvehicle>::iterator it = render->getWorkingWindow()->getObjectStorage()->object3Dvehicles.begin();
			while (it != render->getWorkingWindow()->getObjectStorage()->object3Dvehicles.end()) {
				if (it->second.getType() == "vehicle") {
					graph->getPoint(i)->startConfiguration.push_back(it->first);
				}
				it++;
			}
		}
	}
	std::vector<size_t> endpoints = graph->getEndPoints();
	for (size_t i = 0; i < graph->getPointsNumber(); i++) {
		if (graph->getPoint(i)->isStartPoint()) {
			for (size_t j = 0; j < endpoints.size(); j++) {
				graph->getPoint(i)->endpointsList.push_back(true);
			}
		}
	}
	this->startPoints = graph->getStartPoints();
	this->endPoints = graph->getEndPoints();
}

void Animator::start() {
	int min = 5;
	int max = 16;
	srand(time(NULL));
	int type = rand() % (max - min + 1) + min;
	//std::cout << type << std::endl;
	size_t newVehicleID = render->addVehicle(type);
	size_t id = this->vehicles.size();
	this->vehicles.push_back(Vehicle(this->graph, this->render, startPoints[0], endPoints[0], newVehicleID, id));
	this->isAnimationRunning = true;

	/*startableCarsNumber = 0;
	std::vector<size_t> startableCars;*/
}

void Animator::pause() {

}

void Animator::stop() {
	/*delete this->graph;
	this->isAnimationRunning = false;
	currentIndex = 0;*/
}

size_t Animator::addVehicle(size_t type) {
	size_t vehicle = render->addVehicle(type);

	return vehicle;
}

Graph* Animator::getGraph() {
	return this->graph;
}

void Animator::deleteFinishedVehicles() {
	for (size_t i = 0; i < this->vehicles.size(); i++) {
		if (vehicles[i].isFinished()) {
			this->vehicles[i].erase();
			render->deleteVehicle(this->vehicles[i].getObject3DiD());
		}
	}
}