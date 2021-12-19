/**
 * @name Traffic Simulation
 * @file Vehicle.cpp
 * @class Vehicle
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief Vehicle model representation.
 * Contact: sandorbalazs9402@gmail.com
*/

#include <SDL.h>
#include "Vehicle.h"
#include "../View/Objects/Object3Dvehicle.h"

/**
 * @brief The rapath function flag.
*/
bool Vehicle::repath = true;

/**
 * @brief The default vehicle weight.
*/
int Vehicle::vehicleWeight = 100;

/**
 * @brief Constructor for the Vehicle.
 * @param graph The current graph.
 * @param render The pointer for the render.
 * @param startID The startpoint ID.
 * @param destinationID The destination endpoint.
 * @param object3DiD The 3D render ID.
 * @param ID The current ID.
*/
Vehicle::Vehicle(Graph* graph, Render* render, size_t startID, size_t destinationID, size_t object3DiD, size_t ID) {
	this->graph = graph;
	this->render = render;
	this->object3DiD = object3DiD;
	this->ID = ID;
	this->startID = startID;
	this->destinationID = destinationID;
	this->dijkstra = this->graph->generateDijkstra(startID);
	this->path = this->graph->getPath(this->dijkstra, this->destinationID);
	if (this->path.size() > 0) {
		this->currentRoad = this->path[currentEdgeOnThePath];
		this->graph->getEdge(this->render->getDynamicObject(this->currentRoad)->modelID)->addVehicle(this->vehicleWeight, repath);
		this->firstDirectionCheck();
	} else {
		this->erase();
	}
	this->startTime = SDL_GetTicks();
	this->animator = this->render->getAnimator();
}

/**
 * @brief Getter for the Vehicle ID.
 * @return vehicle ID.
*/
size_t Vehicle::getID() {
	return this->ID;
}

/**
 * @brief Getter for the render ID.
 * @return The render ID.
*/
size_t Vehicle::getObject3DiD() {
	return this->object3DiD;
}

/**
 * @brief Update the Vehicle status.
*/
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

/**
 * @brief Calculate the first start direction and track.
*/
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

/**
 * @brief Check the vehicle direction and track.
*/
char Vehicle::directionCheck() {
	if (currentEdgeOnThePath + 1 != path.size()) {
		if (this->direction == 'a') {
			size_t currentEndpoint = this->graph->getEdge(this->render->getDynamicObject(this->path[currentEdgeOnThePath])->modelID)->getEndpointB();
			size_t nextStartPoint = this->graph->getEdge(this->render->getDynamicObject(this->path[currentEdgeOnThePath + 1])->modelID)->getEndpointA();
			if (currentEndpoint != nextStartPoint) {
				return 'b';
			}
		} else {
			size_t currentEndpoint = this->graph->getEdge(this->render->getDynamicObject(this->path[currentEdgeOnThePath])->modelID)->getEndpointA();
			size_t nextStartPoint = this->graph->getEdge(this->render->getDynamicObject(this->path[currentEdgeOnThePath + 1])->modelID)->getEndpointB();
			if (currentEndpoint != nextStartPoint) {
				return 'a';
			}
		}
	}
	return this->direction;
}

/**
 * @brief Step to the next point.
*/
void Vehicle::nextStep() {
	if (!collisionPrevention) nextRoadIsBlocked = false;
	if (!nextRoadIsBlocked) {
		if (this->direction == 'a') {
			if (this->currentPointOnTheRoad < this->standardRoadLenght - 2) {
				if (!this->collisionTest(this->collisionCheckDistance, this->currentRoad, this->currentPointOnTheRoad, this->direction, this->track)) this->currentPointOnTheRoad++;
			} else {
				this->switchToNextRoad();
			}
		} else {
			if (this->currentPointOnTheRoad > 2) {
				if (!this->collisionTest(this->collisionCheckDistance, this->currentRoad, this->currentPointOnTheRoad, this->direction, this->track)) this->currentPointOnTheRoad--;
			} else {
				this->switchToNextRoad();
			}
		}
		this->checkFinish();
	} else {
		this->switchToNextRoad();
	}
}

/**
 * @brief The collision prevention flag.
*/
bool Vehicle::collisionPrevention = true;

/**
 * @brief  The collision prevention distance.
*/
int Vehicle::collisionCheckDistance = 15;

/**
 * @brief Check the road collision.
*/
bool Vehicle::collisionTest(int checkDistance, size_t localCurrentRoad, size_t localCurrentPointOnTheRoad, char direction, char track) {
	if (collisionPrevention) {
		if (direction == 'a') {
			for (size_t i = 0; i < render->getVehiclesNumber(); i++) {
				if (!render->getVehicle(i)->isDeleted()) {
					for (size_t j = 2; j <= checkDistance; j++) {
						if (localCurrentPointOnTheRoad + j < standardRoadLenght) {
							if (track == '1') {
								if (render->getVehicle(i)->getPosition() == render->getDynamicObject(localCurrentRoad)->trackOne[localCurrentPointOnTheRoad + j]) return true;
							} else {
								if (render->getVehicle(i)->getPosition() == render->getDynamicObject(localCurrentRoad)->trackTwo[localCurrentPointOnTheRoad + j]) return true;
							}
						}
					}
				}
			}
		} else {	
			for (size_t i = 0; i < render->getVehiclesNumber(); i++) {
				if (!render->getVehicle(i)->isDeleted()) {
					for (size_t j = 2; j <= checkDistance; j++) {
						if (localCurrentPointOnTheRoad - j >= 0) {
							if (track == '1') {
								if (render->getVehicle(i)->getPosition() == render->getDynamicObject(localCurrentRoad)->trackOne[localCurrentPointOnTheRoad - j]) return true;
							} else {
								if (render->getVehicle(i)->getPosition() == render->getDynamicObject(localCurrentRoad)->trackTwo[localCurrentPointOnTheRoad - j]) return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}

/**
 * @brief Check the next road.
*/
void Vehicle::switchToNextRoad() {

	char localDirection = directionCheck();
	char localTreck;
	if (localDirection == 'a') localTreck = '1';
	if(localDirection == 'b') localTreck = '2';

	if (!nextRoadIsBlocked && collisionPrevention) {
		if (this->currentEdgeOnThePath + 1 < path.size()) {
			size_t firstPlaceOnTheRoad = 1;
			if (localDirection == 'b') firstPlaceOnTheRoad = this->standardRoadLenght - 2;
			if (this->collisionTest(this->collisionCheckDistance+5, this->path[this->currentEdgeOnThePath + 1], firstPlaceOnTheRoad, localDirection, localTreck)) this->nextRoadIsBlocked = true;
		} else {
			this->nextRoadIsBlocked = false;
		}
	}

	if (!this->nextRoadIsBlocked) {
		this->direction = localDirection;
		this->track = localTreck;

		this->currentEdgeOnThePath++;
		this->hopCounter++;
		this->allCost += this->graph->getEdge(this->render->getDynamicObject(this->currentRoad)->modelID)->getCoast();
		this->graph->getEdge(this->render->getDynamicObject(this->currentRoad)->modelID)->removeVehicle(this->vehicleWeight, repath);
		if (this->currentEdgeOnThePath < path.size()) {
			if (this->direction == 'a') this->currentPointOnTheRoad = 0;
			if (this->direction == 'b') this->currentPointOnTheRoad = this->standardRoadLenght - 1;
			this->currentRoad = this->path[currentEdgeOnThePath];
			this->graph->getEdge(this->render->getDynamicObject(this->currentRoad)->modelID)->addVehicle(this->vehicleWeight, repath);
		}
	}
}

/**
 * @brief Check the path finishing.
*/
void Vehicle::checkFinish() {
	if (this->currentEdgeOnThePath >= path.size()) {
		this->finished = true;
		this->endTime = SDL_GetTicks();
	}
}

/**
 * @brief Getter for the finish flag.
 * @return
*/
bool Vehicle::isFinished() {
	return this->finished;
}

/**
 * @brief Set the vehicle deleted.
*/
void Vehicle::erase() {
	this->deleted = true;
	delete dijkstra;
	graph->getPointByID(this->destinationID)->receivedVehicles++;
}

/**
 * @brief Getter for the deleted flag.
 * @return The deleted flag.
*/
bool Vehicle::isDeleted() {
	return this->deleted;
}

/**
 * @brief Getter for the blocked flag.
 * @return The blocked flag.
*/
bool Vehicle::isBlocked() {
	return nextRoadIsBlocked;
}

/**
 * @brief Unblock the vehicle.
*/
void Vehicle::unblock() {
	this->nextRoadIsBlocked = false;
}