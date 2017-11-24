#include "Slider.h"
#include "Input.h"
#include "Curve.h"
#include "MemoryUtilities.h"
#include <chrono>
#include <thread>

Slider::Slider(int x, int y, unsigned startTime, unsigned endTime, unsigned type,
	wchar_t sliderType, std::vector<vec2<int>> controlPoints)
	: HitObject(x, y, startTime, endTime, type),
	sliderType(sliderType), controlPoints(controlPoints) {
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
	unsigned msPerPoint = (endTime - initialTime + numPoints - 1) / numPoints;

	size_t i = 0;

	while (elapsed < endTime && i < numPoints) {
		Input::moveMouseInstant(sliderPoints[i]);
		i++;
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
