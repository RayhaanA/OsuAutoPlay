#pragma once
#include "vec2.h"
#include <vector>

namespace Curve
{
	vec2<double> linear(std::vector<vec2<double>> controlPoints, double t);

	// Returns points given a t value
	vec2<double> bezier2(std::vector<vec2<double>> controlPoints, double t);
	vec2<double> bezier3(std::vector<vec2<double>> controlPoints, double t);

	// Combines multiple bezier curves
	void bezierPath(std::vector<vec2<double>> &sliderPoints, std::vector<vec2<double>> controlPoints);

	std::vector<vec2<double>> generateSliderPoints(std::vector<vec2<double>> controlPoints, unsigned numPoints);
}

