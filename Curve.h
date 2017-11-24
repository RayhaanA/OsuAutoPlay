#pragma once
#include "vec2.h"
#include <vector>

namespace Curve
{
	vec2<int> linear(std::vector<vec2<int>> controlPoints, double t);

	// Returns points given a t value
	vec2<int> bezier2(std::vector<vec2<int>> controlPoints, double t);
	vec2<int> bezier3(std::vector<vec2<int>> controlPoints, double t);

	// Combines multiple bezier curves
	void bezierPath(std::vector<vec2<int>> &sliderPoints, std::vector<vec2<int>> controlPoints);

	std::vector<vec2<int>> generateSliderPoints(std::vector<vec2<int>> controlPoints, unsigned numPoints);
}

