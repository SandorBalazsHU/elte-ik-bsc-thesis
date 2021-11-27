//SU-30SM
#include "Animator.h"
#include "Render.h"

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

void Animator::start() {
	this->isAnimationRunning = true;
	currentIndex = 0;
	this->graph = new Graph(this->render);
	graph->generateGraph();
}

void Animator::stop() {
	this->isAnimationRunning = false;
	currentIndex = 0;
}