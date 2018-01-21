#include "Slider.h"
#include "Input.h"
#include "Curve.h"
#include "MemoryUtilities.h"
#include <chrono>
#include <thread>

Slider::Slider(double x, double y, unsigned startTime, unsigned endTime, unsigned type,
	wchar_t sliderType, std::vector<vec2<double>> controlPoints, unsigned repeat)
	: HitObject(x, y, startTime, endTime, type),
	sliderType(sliderType), controlPoints(controlPoints), repeat(repeat) {
	// Calculation for curve points here
	sliderPoints = Curve::generateSliderPoints(controlPoints, controlPoints.size());
}


Slider::~Slider() {

}

void Slider::mouseMovement(HANDLE osuProcess, DWORD timeAddress) {
	// Reference to current time
	unsigned initialTime;
	unsigned endTime = getEndTime();
	MemoryUtilities::getElapsedSongTime(osuProcess, timeAddress, initialTime);

	unsigned elapsed = initialTime;
	unsigned numPoints = sliderPoints.size();
	unsigned msPerPoint = (endTime - initialTime + (numPoints * repeat) - 1) / (numPoints * repeat);

	size_t i = 0;
	unsigned repeatIteration = 1;
	int direction = 1;

	while (elapsed < endTime && repeat > 0) {
		if (i == numPoints - 1 || (i == 0 && repeatIteration != 1)) {
			repeat--;
			repeatIteration++;
			if (i == numPoints - 1)
				direction = -1;
			else 
				direction = 1;
		}

		Input::moveMouseInstant(sliderPoints[i]);
		i += direction;

		std::this_thread::sleep_for(std::chrono::milliseconds(msPerPoint));
	}
}


void Slider::printInfo() const {
	std::wcout << "(" << getPosition() << ", " << getStartTime()
		<< ", " << getEndTime() << ", " << getType() << ", " << sliderType << ")" << " # ";
	for (const auto & i : sliderPoints) {
		std::wcout << i << " | ";
	}
}
