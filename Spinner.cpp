#include "Spinner.h"
#include "MemoryUtilities.h"
#include "Input.h"
#include <thread>
#include <chrono>

Spinner::Spinner(double x, double y, int startTime, int endTime, unsigned type)
	: HitObject(x, y, startTime, endTime, type) {}

Spinner::~Spinner() {
}

void Spinner::mouseMovement(HANDLE osuProcess, DWORD timeAddress) {
	// Initialize to spinner center first
	vec2<double> start = this->getPosition();
	Input::moveMouseInstant(start);
	
	int currentTime;
	MemoryUtilities::getElapsedSongTime(osuProcess, timeAddress, currentTime);
	unsigned i = 0;
	const double PI = 3.14159265;
	double rad;
	int radius = 100;

	// Moves mouse around unit circle based on i and the division factor 
	while (currentTime < getEndTime()) {
		MemoryUtilities::getElapsedSongTime(osuProcess, timeAddress, currentTime);
		if (i == 6) 
			i = 0;
		rad = PI * i / 3;
		Input::moveMouseInstant(vec2<double>{ (start.getX() + (radius * std::cosf(rad))),
												(start.getY() + (radius * std::sinf(rad))) });
		i++;
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}
}

void Spinner::printInfo() const {
	std::wcout << "(" << getPosition() << ", " << getStartTime()
		<< ", " << getEndTime() << ", " << getType() << ")";
}