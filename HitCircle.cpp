#include "HitCircle.h"
#include <iostream>

HitCircle::HitCircle(unsigned x, unsigned y, unsigned startTime,  unsigned type, unsigned endTime)
  : HitObject(x, y, startTime, endTime, type) {}


HitCircle::~HitCircle() {

}

void HitCircle::hit() {

}

void HitCircle::printInfo() const {
	std::cout << "(" << getX() << ", " << getY() << ", " << getStartTime()
		<< ", " << getEndTime() << ", " << getType() << ")";
}
