#include "Vehicle.h"
#include "../View/Objects/Object3Dvehicle.h"

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
	if (repath) this->graph->getEdge(this->render->getDynamicObject(this->currentRoad)->modelID)->addVehicleCoast(this->vehicleWeight);
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
			render->getDynamicObject(currentRoad)->trackOne[this->currentPointOnTheRoad - 1]), 0, 1, 0));
	}
}

void Vehicle::nextStep() {
	if (this->currentPointOnTheRoad < this->standardRoadLenght - 1) {
		currentPointOnTheRoad++;
	} else {
		this->currentEdgeOnThePath++;
		if (this->currentEdgeOnThePath < path.size()) {
			this->currentPointOnTheRoad = 0;
			if (repath) this->graph->getEdge(this->render->getDynamicObject(this->currentRoad)->modelID)->removeVehicleCoast(this->vehicleWeight);
			this->currentRoad = this->path[currentEdgeOnThePath];
			if (repath) this->graph->getEdge(this->render->getDynamicObject(this->currentRoad)->modelID)->addVehicleCoast(this->vehicleWeight);
		}
	}
	if (this->currentEdgeOnThePath >= path.size()) this->finished = true;
}

bool Vehicle::isFinished() {
	return this->finished;
}

void Vehicle::erase() {
	this->deleted = true;
}

bool Vehicle::isDeleted() {
	return this->deleted;
}