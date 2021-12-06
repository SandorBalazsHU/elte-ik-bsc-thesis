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
		Point* point = graph->getPointByID(startPoints[i]);
		if (point->startableVehicles > 0) {
			addVehicle(startPoints[i]);
			point->startableVehicles -= 1;
			/*std::cout << std::endl;
			for (size_t i = 0; i < point->startConfiguration.size(); i++) {
				std::cout << point->startConfiguration[i] << std::endl;
			}*/
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