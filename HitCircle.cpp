#include "HitCircle.h"
#include "Input.h"
#include <iostream>

HitCircle::HitCircle(double x, double y, unsigned startTime, unsigned endTime, unsigned type)
  : HitObject(x, y, startTime, endTime, type) {}


HitCircle::~HitCircle() {

}

void HitCircle::mouseMovement(HANDLE osuProcess, DWORD timeAddress) {
	Input::moveMouseInstant(getPosition());
}

void HitCircle::printInfo() const {
	std::wcout << "(" << getPosition() << ", " << getStartTime()
		<< ", " << getEndTime() << ", " << getType() << ")";
}
