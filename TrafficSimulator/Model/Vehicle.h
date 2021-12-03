#pragma once

class Vehicle {
public:
	Vehicle(size_t object3DiD, size_t ID);
	size_t getID();
	size_t getObject3DiD();
private:
	size_t object3DiD;
	size_t ID;
};