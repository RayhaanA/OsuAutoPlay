#include "HitCircle.h"
#include "Input.h"
#include <iostream>

HitCircle::HitCircle(unsigned x, unsigned y, unsigned startTime, unsigned endTime, unsigned type)
  : HitObject(x, y, startTime, endTime, type) {}


HitCircle::~HitCircle() {

}

void HitCircle::mouseMovement() {
	Input::moveMouseInstant(getPosition());
}

void HitCircle::printInfo() const {
	std::wcout << "(" << getPosition() << ", " << getStartTime()
		<< ", " << getEndTime() << ", " << getType() << ")";
}
