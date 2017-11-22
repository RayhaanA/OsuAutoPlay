#include "Slider.h"
#include "Input.h"

Slider::Slider(int x, int y, unsigned startTime, unsigned endTime, unsigned type,
	wchar_t sliderType, std::vector<vec2<int>> controlPoints)
	: HitObject(x, y, startTime, endTime, type),
	sliderType(sliderType), controlPoints(controlPoints) {
	// Calculation for curve points here
	// sliderPoints = 
}


Slider::~Slider() {

}

void Slider::mouseMovement(DWORD timeAddress) {
	Input::moveMouseInstant(this->getPosition());
}

void Slider::printInfo() const {
	std::wcout << "(" << getPosition() << ", " << getStartTime()
		<< ", " << getEndTime() << ", " << getType() << ", " << sliderType << ")" << " # ";
	for (const auto & i : controlPoints) {
		std::wcout << i << " | ";
	}
}

void Slider::setSliderPoints(std::vector<vec2<int>> points) {
	sliderPoints = points;
}