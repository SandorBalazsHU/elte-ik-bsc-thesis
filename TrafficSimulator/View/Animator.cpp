//SU-30SM
#include "Animator.h"
#include "Render.h"
#include "WorkWindow.h"

Animator::Animator(void) {

}

Animator::~Animator(void) {

}

void Animator::bind(Render* render) {
	this->render = render;
}

void Animator::update() {
	timerUpdate();
	if (render->getDynamicObjectsNumber() > 0 && this->isAnimationRunning) {
		render->getVehicle(0)->setPosition(render->getDynamicObject(0)->trackOne[currentIndex]);
		render->getVehicle(1)->setPosition(render->getDynamicObject(0)->trackTwo[currentIndex]);
		render->getVehicle(0)->setRotation(glm::vec4(Object3Dvehicle::getMoveRtation(render->getDynamicObject(0)->trackOne[currentIndex], render->getDynamicObject(0)->trackOne[currentIndex + 1]), 0, 1, 0));
		render->getVehicle(1)->setRotation(glm::vec4(Object3Dvehicle::getMoveRtation(render->getDynamicObject(0)->trackTwo[currentIndex], render->getDynamicObject(0)->trackTwo[currentIndex + 1]), 0, 1, 0));
	}
}

void Animator::timerUpdate() {
	currentTime = SDL_GetTicks();
	if (currentTime - prevouseTime >= 20 && this->isAnimationRunning) {
		if (currentIndex < 100 - 2 && this->isAnimationRunning) {
			currentIndex++;
			std::cout << currentIndex << std::endl;
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
	std::vector<size_t> endpoints = graph->getStartPoints();
	for (size_t i = 0; i < graph->getPointsNumber(); i++) {
		if (graph->getPoint(i)->isStartPoint()) {
			for (size_t j = 0; j < endpoints.size(); j++) {
				graph->getPoint(i)->endpointsList.push_back(true);
			}
		}
	}
}

void Animator::start() {
	this->isAnimationRunning = true;

	/*startableCarsNumber = 0;
	std::vector<size_t> startableCars;*/
}

void Animator::pause() {

}

void Animator::stop() {
	delete this->graph;
	this->isAnimationRunning = false;
	currentIndex = 0;
}

size_t Animator::addVehicle(size_t type) {
	size_t vehicle = render->addVehicle(type);

	return vehicle;
}

Graph* Animator::getGraph() {
	return this->graph;
}