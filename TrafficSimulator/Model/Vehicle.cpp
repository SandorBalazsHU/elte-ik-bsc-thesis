#include <SDL.h>
#include "Vehicle.h"
#include "../View/Objects/Object3Dvehicle.h"

bool Vehicle::repath = true;
int Vehicle::vehicleWeight = 100;

Vehicle::Vehicle(Graph* graph, Render* render, size_t startID, size_t destinationID, size_t object3DiD, size_t ID) {
	this->graph = graph;
	this->render = render;
	this->object3DiD = object3DiD;
	this->ID = ID;
	this->startID = startID;
	this->destinationID = destinationID;
	this->dijkstra = this->graph->generateDijkstra(startID);
	this->path = this->graph->getPath(this->dijkstra, this->destinationID);
	this->currentRoad = this->path[currentEdgeOnThePath];
	this->graph->getEdge(this->render->getDynamicObject(this->currentRoad)->modelID)->addVehicle(this->vehicleWeight, repath);
	this->firstDirectionCheck();
	this->startTime = SDL_GetTicks();
}

size_t Vehicle::getID() {
	return this->ID;
}

size_t Vehicle::getObject3DiD() {
	return this->object3DiD;
}

void Vehicle::update() {
	Object3Dvehicle* vehicle = render->getVehicle(this->object3DiD);
	nextStep();
	if (this->track == '1') {
		vehicle->setPosition(render->getDynamicObject(currentRoad)->trackOne[this->currentPointOnTheRoad]);
		if (this->direction == 'a') vehicle->setRotation(glm::vec4(Object3Dvehicle::getMoveRtation(render->getDynamicObject(currentRoad)->trackOne[this->currentPointOnTheRoad],
			render->getDynamicObject(currentRoad)->trackOne[this->currentPointOnTheRoad + 1]), 0, 1, 0));
		if (this->direction == 'b') vehicle->setRotation(glm::vec4(Object3Dvehicle::getMoveRtation(render->getDynamicObject(currentRoad)->trackOne[this->currentPointOnTheRoad],
			render->getDynamicObject(currentRoad)->trackOne[this->currentPointOnTheRoad - 1]), 0, 1, 0));
	}
	if (this->track == '2') {
		vehicle->setPosition(render->getDynamicObject(currentRoad)->trackTwo[this->currentPointOnTheRoad]);
		if (this->direction == 'a') vehicle->setRotation(glm::vec4(Object3Dvehicle::getMoveRtation(render->getDynamicObject(currentRoad)->trackTwo[this->currentPointOnTheRoad],
			render->getDynamicObject(currentRoad)->trackOne[this->currentPointOnTheRoad + 1]), 0, 1, 0));
		if (this->direction == 'b') vehicle->setRotation(glm::vec4(Object3Dvehicle::getMoveRtation(render->getDynamicObject(currentRoad)->trackTwo[this->currentPointOnTheRoad],
			render->getDynamicObject(currentRoad)->trackTwo[this->currentPointOnTheRoad - 1]), 0, 1, 0));
	}
}

void Vehicle::firstDirectionCheck() {
	size_t firstEdgeEndpointA = this->graph->getPointByID(this->graph->getEdge(this->render->getDynamicObject(this->path[currentEdgeOnThePath])->modelID)->getEndpointA())->getID();
	if (this->startID ==  firstEdgeEndpointA) {
		this->direction = 'a';
		this->track = '1';
	}else{
		this->direction = 'b';
		this->track = '2';
	}
	if (this->direction == 'b') this->currentPointOnTheRoad = this->standardRoadLenght - 1;
}

void Vehicle::directionCheck() {
	if (currentEdgeOnThePath + 1 != path.size()) {
		if (this->direction == 'a') {
			size_t currentEndpoint = this->graph->getEdge(this->render->getDynamicObject(this->path[currentEdgeOnThePath])->modelID)->getEndpointB();
			size_t nextStartPoint = this->graph->getEdge(this->render->getDynamicObject(this->path[currentEdgeOnThePath + 1])->modelID)->getEndpointA();
			if (currentEndpoint != nextStartPoint) {
				this->direction = 'b';
				this->track = '2';
			}
		} else {
			size_t currentEndpoint = this->graph->getEdge(this->render->getDynamicObject(this->path[currentEdgeOnThePath])->modelID)->getEndpointA();
			size_t nextStartPoint = this->graph->getEdge(this->render->getDynamicObject(this->path[currentEdgeOnThePath + 1])->modelID)->getEndpointB();
			if (currentEndpoint != nextStartPoint) {
				this->direction = 'a';
				this->track = '1';
			}
		}
	}
}

void Vehicle::nextStep() {
	if (this->direction == 'a') {
		//2???
		if (this->currentPointOnTheRoad < this->standardRoadLenght - 2) {
			currentPointOnTheRoad++;
		} else {
			switchToNextRoad();
		}
	} else {
		if (this->currentPointOnTheRoad > 1) {
			currentPointOnTheRoad--;
		}else{
			switchToNextRoad();
		}
	}
	checkFinish();
}

void Vehicle::switchToNextRoad() {
	directionCheck();
	this->currentEdgeOnThePath++;
	hopCounter++;
	this->graph->getEdge(this->render->getDynamicObject(this->currentRoad)->modelID)->removeVehicle(this->vehicleWeight, repath);
	if (this->currentEdgeOnThePath < path.size()) {
		if (this->direction == 'a') this->currentPointOnTheRoad = 0;
		if (this->direction == 'b') this->currentPointOnTheRoad = this->standardRoadLenght-1;
		this->currentRoad = this->path[currentEdgeOnThePath];
		this->graph->getEdge(this->render->getDynamicObject(this->currentRoad)->modelID)->addVehicle(this->vehicleWeight, repath);
	}
}

void Vehicle::checkFinish() {
	if (this->currentEdgeOnThePath >= path.size()) {
		this->finished = true;
		this->endTime = SDL_GetTicks();
	}
}

bool Vehicle::isFinished() {
	return this->finished;
}

void Vehicle::erase() {
	this->deleted = true;
	delete dijkstra;
	graph->getPointByID(this->destinationID)->receivedVehicles++;
}

bool Vehicle::isDeleted() {
	return this->deleted;
}