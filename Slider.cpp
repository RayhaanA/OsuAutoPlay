#include "Slider.h"
#include "Input.h"

Slider::Slider(unsigned x, unsigned y, unsigned startTime, unsigned endTime, unsigned type, 
	wchar_t sliderType, std::vector<vec2<unsigned>> controlPoints)
	: HitObject(x, y, startTime, endTime, type),
	sliderType(sliderType), controlPoints(controlPoints) {
	// Calculation for curve points here
	// sliderPoints = 
}


Slider::~Slider() {

}

void Slider::mouseMovement() {
	Input::moveMouseInstant(this->getPosition());
}

void Slider::printInfo() const {
	std::wcout << "(" << getPosition() << ", " << getStartTime()
		<< ", " << getEndTime() << ", " << getType() << ", " << sliderType << ")" << " # ";
	for (const auto & i : controlPoints) {
		std::wcout << i << " | ";
	}
}

void Slider::setSliderPoints(std::vector<vec2<unsigned>> points) {
	sliderPoints = points;
}