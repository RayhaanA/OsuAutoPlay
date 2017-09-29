#include "Spinner.h"

Spinner::Spinner(unsigned x, unsigned y, unsigned startTime, unsigned endTime, unsigned type) 
	: HitObject(x, y, startTime, endTime, type) {}

Spinner::~Spinner() {
}

void Spinner::hit() {

}

void Spinner::printInfo() const {
	std::cout << "(" << getX() << ", " << getY() << ", " << getStartTime()
		<< ", " << getEndTime() << ", " << getType() << ")";
}