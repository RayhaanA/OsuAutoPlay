#pragma once
#include "HitObject.h"
#include "vec2.h"
#include <vector>

class Slider :
	public HitObject
{
public:
	Slider(unsigned x, unsigned y, unsigned startTime, unsigned endTime, unsigned type,
		 wchar_t sliderType, std::vector<vec2<unsigned>> controlPoints);
	~Slider();

	void hit();

	void printInfo() const;

private:
	wchar_t sliderType;
	std::vector<vec2<unsigned>> controlPoints;
	unsigned repeat;
	double pixelLength;
};

