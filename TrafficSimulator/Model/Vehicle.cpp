#include "Vehicle.h"

Vehicle::Vehicle(size_t object3DiD, size_t ID) {
	this->object3DiD = object3DiD;
	this->ID = ID;
}

size_t Vehicle::getID() {
	return this->ID;
}

size_t Vehicle::getObject3DiD() {
	this->object3DiD;
}