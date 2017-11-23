#include "Spinner.h"
#include "MemoryUtilities.h"
#include "Input.h"
#include <thread>
#include <chrono>

Spinner::Spinner(int x, int y, unsigned startTime, unsigned endTime, unsigned type)
	: HitObject(x, y, startTime, endTime, type) {}

Spinner::~Spinner() {
}

void Spinner::mouseMovement(HANDLE osuProcess, DWORD timeAddress) {
	// Initialize to spinner center first
	vec2<int> start = this->getPosition();
	Input::moveMouseInstant(start);
	
	unsigned currentTime = MemoryUtilities::getElapsedSongTime(osuProcess, timeAddress, currentTime);
	unsigned i = 0;
	const float PI = 3.14159265;
	float rad;
	int radius = 10;

	// Moves mouse around unit circle based on i and the division factor 
	while (currentTime < getEndTime()) {
		if (i == 6) i = 0;
		rad = PI * i / 3;
		Input::moveMouseInstant(vec2<int>{ static_cast<int>(start.getX() + (radius * std::cosf(rad))), 
										   static_cast<int>(start.getY() + (radius * std::sinf(rad))) });
		i++;
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}
}

void Spinner::printInfo() const {
	std::wcout << "(" << getPosition() << ", " << getStartTime()
		<< ", " << getEndTime() << ", " << getType() << ")";
}