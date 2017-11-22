#include "Spinner.h"
#include "Input.h"

Spinner::Spinner(int x, int y, unsigned startTime, unsigned endTime, unsigned type)
	: HitObject(x, y, startTime, endTime, type) {}

Spinner::~Spinner() {
}

void Spinner::mouseMovement(DWORD timeAddress) {
	Input::moveMouseInstant(this->getPosition());
}

void Spinner::printInfo() const {
	std::wcout << "(" << getPosition() << ", " << getStartTime()
		<< ", " << getEndTime() << ", " << getType() << ")";
}