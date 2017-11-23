#include "Curve.h"

const unsigned int NUM_SMOOTHING_ITERATIONS = 10;

vec2<int> Curve::linear(std::vector<vec2<int>> controlPoints, double t)
{
	vec2<int> x = controlPoints[0];
	vec2<int> y = controlPoints[1];

	if (x == y)
		return x;

	int xDir = static_cast<int>(y.getX()) - static_cast<int>(x.getX());
	int yDir = static_cast<int>(y.getY()) - static_cast<int>(x.getY());

	// unsigned magnitude = (y - x).magnitude();
	
	return vec2<int>(x.getX() + static_cast<int>(xDir * t), x.getY() + static_cast<int>(yDir * t));
}

vec2<int> Curve::bezier2(std::vector<vec2<int>> controlPoints, double t)
{
	vec2<int> x = controlPoints[0];
	vec2<int> y = controlPoints[1];
	vec2<int> z = controlPoints[2];

	double t2 = t * t;
	double mt = 1 - t;
	double mt2 = mt * mt;

	int a = static_cast<int>(mt2 * x.getX() + 2 * mt * t * y.getX() + t2 * z.getX());
	int b = static_cast<int>(mt2 * x.getY() + 2 * mt * t * y.getY() + t2 * z.getY());

	return vec2<int>{ a, b };
}

vec2<int> Curve::bezier3(std::vector<vec2<int>> controlPoints, double t)
{
	vec2<int> w = controlPoints[0];
	vec2<int> x = controlPoints[1];
	vec2<int> y = controlPoints[2];
	vec2<int> z = controlPoints[3];

	double t2 = t * t;
	double t3 = t2 * t;
	double mt = 1 - t;
	double mt2 = mt * mt;
	double mt3 = mt2 * mt;

	int a = static_cast<int>(mt3 * w.getX() + 3 * mt2 * t * x.getX() + 3 * mt2 * t2 * y.getX() + t3 * z.getX());
	int b = static_cast<int>(mt3 * w.getY() + 3 * mt2 * t * x.getY() + 3 * mt2 * t2 * y.getY() + t3 * z.getY());

	return vec2<int>{ a, b };
}

std::vector<vec2<int>> Curve::bezierPath(std::vector<vec2<int>> controlPoints)
{	
	// osu! sliders made up of more than one line/curve are denoted by repeated sequential points
	// where one curve ends and the next one starts. To coalesce curves simply calculate 
	// all the slider points for each curve segment and add them to one array

	// Walkthrough control points and form curves for each segment
	std::vector<vec2<int>> sliderPoints = {};

	// TODO: Maybe look at fixing repetition in this segment
	for (unsigned start = 0, end = 1; end < controlPoints.size(); end++) {
		// Make sure we're not at the end of the list and only one left
		if (start == controlPoints.size() - 1 && end == controlPoints.size() - 1) {
			sliderPoints.push_back(controlPoints[end]);
		}
		// Peek to see if start of next curve
		else if ((end != controlPoints.size() - 1 && controlPoints[end] == controlPoints[end + 1]) || end == controlPoints.size() - 1) {
			if (end - start < 2) { // 2 total points -> linear
				auto points = std::vector<vec2<int>>{ controlPoints[start], controlPoints[end] };
				for (unsigned i = 0; i <= NUM_SMOOTHING_ITERATIONS; i++) {
					sliderPoints.push_back(linear(points, 1.0 / NUM_SMOOTHING_ITERATIONS * i));
				}
			}
			else if (end - start < 3) { // 3 total points -> bezier2
				auto points = std::vector<vec2<int>>{ controlPoints[start], controlPoints[start + 1], controlPoints[end] };
				for (unsigned i = 0; i <= NUM_SMOOTHING_ITERATIONS; i++) {
					sliderPoints.push_back(bezier2(points, 1.0 / NUM_SMOOTHING_ITERATIONS * i));
				}
			}
			else { // 4 total points -> bezier3
				auto points = std::vector<vec2<int>>{ controlPoints[start], controlPoints[start + 1], controlPoints[start+2], controlPoints[end] };
				for (unsigned i = 0; i <= NUM_SMOOTHING_ITERATIONS; i++) {
					sliderPoints.push_back(bezier3(points, 1.0 / NUM_SMOOTHING_ITERATIONS * i));
				}
			}
			start = end + 1; // Update to the start of the new path component
		}
	}

	return sliderPoints;
}

std::vector<vec2<int>> Curve::generateSliderPoints(std::vector<vec2<int>> controlPoints, unsigned numPoints)
{
	std::vector<vec2<int>> sliderPoints;
	
	switch (numPoints) {
	case 2:
		for (unsigned i = 0; i < NUM_SMOOTHING_ITERATIONS; i++) {
			sliderPoints.push_back(linear(controlPoints, 1.0 / NUM_SMOOTHING_ITERATIONS * i));
		}
		break;
	case 3:
		for (unsigned i = 0; i < NUM_SMOOTHING_ITERATIONS; i++) {
			sliderPoints.push_back(bezier2(controlPoints, 1.0 / NUM_SMOOTHING_ITERATIONS * i));
		}
		break;
	case 4:
		for (unsigned i = 0; i < NUM_SMOOTHING_ITERATIONS; i++) {
			sliderPoints.push_back(bezier3(controlPoints, 1.0 / NUM_SMOOTHING_ITERATIONS * i));
		}
		break;
	default:
		sliderPoints = bezierPath(controlPoints);
	}

	return sliderPoints;
}
