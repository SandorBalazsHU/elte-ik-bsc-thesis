/**
 * @name Traffic Simulation
 * @file Animator.cpp
 * @class Animator
 * @author Sándor Balázs - AZA6NL
 * @date 2021.12.15.
 * @brief The animation controller class.
 * Contact: sandorbalazs9402@gmail.com
*/

//SU-30SM
//Never Fade Away

#include <stdlib.h>
#include <time.h>
#include "Animator.h"
#include "Render.h"
#include "WorkWindow.h"
#include "../Model/Vehicle.h"
#include "../Model/Point.h"

/**
 * @brief Animator constructor. Empty.
 * @param  Empty.
*/
Animator::Animator(void) {

}

/**
 * @brief Animator destructor. Empty.
 * @param  Empty.
*/
Animator::~Animator(void) {

}

/**
 * @brief Static animator update frequency.
*/
int Animator::updateFrequency = 20;

/**
 * @brief Static animator starter update frequency.
*/
int Animator::vehicleStarterUpdateFrequency = 1000;

/**
 * @brief Prepare the Animator class.
 * @param render The current render object.
*/
void Animator::bind(Render* render) {
	this->render = render;
}

/**
 * @brief The animation update. Callable by every frame for the animation update.
*/
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
		if (Vehicle::collisionPrevention) {
			updateStuckVehicleList();
			if (!stuckVehicles.empty()) {
				size_t minID = *stuckVehicles.begin();
				this->vehicles[minID].unblock();
				stuckVehicles.erase(minID);
			}
			if (!stuckVehicles.empty()) {
				size_t randomID = getRandomNumber(0, stuckVehicles.size() - 1);
				auto it = stuckVehicles.begin();
				for (int i = 0; i < randomID; i++) {
					it++;
				}
				size_t minID = *it;
				this->vehicles[minID].unblock();
				stuckVehicles.erase(minID);
			}
		}
	}
	deleteFinishedVehicles();
}

/**
 * @brief The animation timer calculator.
 * @return The clock tick for the animation timing.
*/
bool Animator::animationTimer() {
	bool isTime = false;
	currentTime = SDL_GetTicks();
	if (currentTime - prevouseTime >= updateFrequency) {
		prevouseTime = SDL_GetTicks();
		isTime = true;
	}
	return isTime;
}

/**
 * @brief The vehicle adding timer calculator.
 * @return The clock tick for the vehicle start timing. 
*/
bool Animator::vehicleStarterTimer() {
	bool isTime = false;
	vehicleStarterCurrentTime = SDL_GetTicks();
	if (vehicleStarterCurrentTime - vehicleStarterPrevouseTime >= vehicleStarterUpdateFrequency) {
		vehicleStarterPrevouseTime = SDL_GetTicks();
		isTime = true;
	}
	return isTime;
}

/**
 * @brief Prepare the graph representation and all variable for the animation and the vehicle path finding.
*/
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

/**
 * @brief Start the animation running.
*/
void Animator::start() {
	this->isAnimationRunning = true;
}

/**
 * @brief Pause the animation.
*/
void Animator::pause() {
	this->isAnimationRunning = false;
	this->softRunning = false;
}

/**
 * @brief Stop the animation running.
*/
void Animator::stop() {
	this->isAnimationRunning = false;
	this->softRunning = true;
	startedVehiclesIndex = 0;
	render->clearVehicles();
	this->vehicles.clear();
	this->resetStats();
}

/**
 * @brief Reset the statistic counters.
*/
void  Animator::resetStats() {
	for (size_t i = 0; i < graph->getEdgesNumber(); i++) {
		graph->getEdge(i)->resetStats();
	}
	for (size_t i = 0; i < graph->getPointsNumber(); i++) {
		graph->getPoint(i)->resetStats();
	}
}

/**
 * @brief Vehicle adder method. Create the 3D and the abstract vehicle too.
 * @param startPoint The startable vehicle start point.
 * @param singleVehicle The startable vehicle destination point.
*/
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

/**
 * @brief Random generator.
 * @param min The needed minimum random number.
 * @param max The needed maximum random number.
 * @return The generated random number.
*/
int Animator::getRandomNumber(int min, int max) {
	srand(time(NULL));
	if(min < max){
		return rand() % (max - min + 1) + min;
	} else {
		return min;
	}
	
}

/**
 * @brief Timed automate timed secvencial vehicle adder.
*/
void Animator::autoAdder() {
	Point* point = graph->getPointByID(startPoints[startedVehiclesIndex]);
	if (point->startableVehicles > 0) {
		if (!startCollisionDetection()) {
			addVehicle(startPoints[startedVehiclesIndex], false);
			point->startableVehicles -= 1;
		}
	}
	startedVehiclesIndex++;
	if (startedVehiclesIndex == this->startPoints.size()) startedVehiclesIndex = 0;
}

/**
 * @brief Getter for the generated graph. The graph generated in the finalize method.
 * @return The generated graph
*/
Graph* Animator::getGraph() {
	return this->graph;
}

/**
 * @brief Delete the finished vehicles.
*/
void Animator::deleteFinishedVehicles() {
	for (size_t i = 0; i < this->vehicles.size(); i++) {
		if (vehicles[i].isFinished() && !vehicles[i].isDeleted()) {
			this->vehicles[i].erase();
			render->deleteVehicle(this->vehicles[i].getObject3DiD());
		}
	}
}

/**
 * @brief Delete all abstract vehicle.
*/
void Animator::clear() {
	this->vehicles.clear();
}

/**
 * @brief Getter for an abstract vehicle
 * @param modelID The needed abstract vehicle.
 * @return The abstract vehicle.
*/
Vehicle* Animator::getVehicleModel(size_t modelID) {
	return &this->vehicles[modelID];
}

/**
 * @brief Getter for the generated abstract vehicles number.
 * @return The generated abstract vehicles number.
*/
size_t Animator::getVehicleModelSize() {
	return this->vehicles.size();
}

/**
 * @brief Update the stuck vehicles list.;
*/
void Animator::updateStuckVehicleList() {
	for (size_t i = 0; i < vehicles.size(); i++) {
		if (!this->vehicles[i].isDeleted()) {
			if (this->vehicles[i].isBlocked()) this->stuckVehicles.insert(i);
		}
	}
}

/**
 * @brief Check the start road to collisions.
 * @return Return true if the start road is not free.
*/
bool Animator::startCollisionDetection() {
	if (Vehicle::collisionPrevention) {
		char direction = graph->getPointByID(startPoints[startedVehiclesIndex])->getSide();
		size_t currentRoadID = graph->getEdgeByID(graph->getPointByID(startPoints[startedVehiclesIndex])->getEdge())->getRoad3DiD();

		if (direction == 'a') {
			for (size_t i = 0; i < render->getVehiclesNumber(); i++) {
				if (!render->getVehicle(i)->isDeleted()) {
					for (size_t j = 2; j <= Vehicle::collisionCheckDistance; j++) {
						if (render->getVehicle(i)->getPosition() == render->getDynamicObject(currentRoadID)->trackOne[j]) return true;
					}
				}
			}
		}
		else {
			for (size_t i = 0; i < render->getVehiclesNumber(); i++) {
				if (!render->getVehicle(i)->isDeleted()) {
					for (size_t j = 2; j <= Vehicle::collisionCheckDistance; j++) {
						if (render->getVehicle(i)->getPosition() == render->getDynamicObject(currentRoadID)->trackTwo[100 - j]) return true;
					}
				}
			}
		}
	}
	return false;
}