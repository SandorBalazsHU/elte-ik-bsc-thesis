//SU-30SM
//Never Fade Away
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
	this->isAnimationRunning = true;
}

void Animator::pause() {
	this->isAnimationRunning = false;
}

void Animator::stop() {
	this->isAnimationRunning = false;
	/*delete this->graph;
	this->isAnimationRunning = false;
	currentIndex = 0;*/
}

void Animator::addVehicle(size_t startPoint, size_t endPoint) {
	if (render->isEditorLoced()) {
		if (startPoint == -1) {
			startPoint = startPoints[0];
		}
		if (endPoint == -1) {
			endPoint = endPoints[0];
		}
		int min = 5;
		int max = 16;
		srand(time(NULL));
		int type = rand() % (max - min + 1) + min;
		size_t newVehicleID = render->addVehicle(type);
		size_t id = this->vehicles.size();
		this->vehicles.push_back(Vehicle(this->graph, this->render, startPoint, endPoint, newVehicleID, id));
		this->vehicles[this->vehicles.size() - 1].update();
		this->softRunning = true;
	}
}

void Animator::autoAdder() {
	for (size_t i = 0; i < this->startPoints.size(); i++) {
		if (graph->getPointByID(startPoints[i])->startableVehicles > 0) {
			addVehicle(startPoints[i]);
			graph->getPointByID(startPoints[i])->startableVehicles -= 1;
		}
	}
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

void Animator::clear() {
	this->vehicles.clear();
}