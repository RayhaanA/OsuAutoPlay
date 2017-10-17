#pragma once
#include "vec2.h"
#include <vector>

namespace Curve
{
	vec2<unsigned> linear(std::vector<vec2<unsigned>> controlPoints, double t);

	// Returns points given a t value
	vec2<unsigned> bezier2(std::vector<vec2<unsigned>> controlPoints, double t);
	vec2<unsigned> bezier3(std::vector<vec2<unsigned>> controlPoints, double t);

	// Combines multiple bezier curves
	std::vector<vec2<unsigned>> bezierPath(std::vector<vec2<unsigned>> controlPoints);

	std::vector<vec2<unsigned>> generateSliderPoints(std::vector<vec2<unsigned>> controlPoints, unsigned numPoints);
}

