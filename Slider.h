#pragma once
#include "HitObject.h"
#include "vec2.h"
#include <vector>

class Slider :
	public HitObject
{
public:
	Slider(unsigned x, unsigned y, unsigned startTime, unsigned endTime, unsigned type);
	~Slider();

	void addControlPoint(vec2<unsigned> point);

	void hit();

private:
	std::vector<vec2<unsigned>> controlPoints;
};

