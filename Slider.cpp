#include "Slider.h"

Slider::Slider(unsigned x, unsigned y, unsigned startTime, unsigned endTime, unsigned type, unsigned repeat, double pixelLength)
	: HitObject(x, y, startTime, endTime, type), repeat(repeat), pixelLength(pixelLength) {}


Slider::~Slider() {

}

void Slider::addControlPoint(vec2<unsigned> point) {
	controlPoints.push_back(point);
}

void Slider::hit() {

}
