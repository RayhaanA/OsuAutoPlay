#include "Curve.h"

vec2<unsigned> Curve::linear(std::vector<vec2<unsigned>> controlPoints, double t)
{
	vec2<unsigned> x = controlPoints.at(0);
	vec2<unsigned> y = controlPoints.at(1);

	int xDir = static_cast<int>(y.getX()) - static_cast<int>(x.getX());
	int yDir = static_cast<int>(y.getY()) - static_cast<int>(x.getY());

	// unsigned magnitude = (y - x).magnitude();
	
	return vec2<unsigned>(x.getX() + static_cast<unsigned>(xDir * t), x.getY() + static_cast<unsigned>(yDir * t));
}

vec2<unsigned> Curve::bezier2(std::vector<vec2<unsigned>> controlPoints, double t)
{
	vec2<unsigned> x = controlPoints.at(0);
	vec2<unsigned> y = controlPoints.at(1);
	vec2<unsigned> z = controlPoints.at(2);

	double t2 = t * t;
	double mt = 1 - t;
	double mt2 = mt * mt;

	unsigned a = static_cast<unsigned>(mt2 * x.getX() + 2 * mt * t * y.getX() + t2 * z.getX());
	unsigned b = static_cast<unsigned>(mt2 * x.getY() + 2 * mt * t * y.getY() + t2 * z.getY());

	return vec2<unsigned>{ a, b };
}

vec2<unsigned> Curve::bezier3(std::vector<vec2<unsigned>> controlPoints, double t)
{
	vec2<unsigned> w = controlPoints.at(0);
	vec2<unsigned> x = controlPoints.at(1);
	vec2<unsigned> y = controlPoints.at(2);
	vec2<unsigned> z = controlPoints.at(3);

	double t2 = t * t;
	double t3 = t2 * t;
	double mt = 1 - t;
	double mt2 = mt * mt;
	double mt3 = mt2 * mt;

	unsigned a = static_cast<unsigned>(mt3 * w.getX() + 3 * mt2 * t * x.getX() + 3 * mt2 * t2 * y.getX() + t3 * z.getX());
	unsigned b = static_cast<unsigned>(mt3 * w.getY() + 3 * mt2 * t * x.getY() + 3 * mt2 * t2 * y.getY() + t3 * z.getY());

	return vec2<unsigned>{ a, b };
}

std::vector<vec2<unsigned>> Curve::bezierPath(std::vector<vec2<unsigned>> controlPoints)
{	
	// osu! sliders made up of more than one line/curve are denoted by repeated sequential points
	// where one curve ends and the next one starts. To coalesce curves simply calculate 
	// all the slider points for each curve segment and add them to one array

	// Walkthrough control points and form curves for each segment
	unsigned i = 0;
	unsigned j = 1;

	for (; j < controlPoints.size(); j++) {
		// Peek to see if start of next curve
		if (controlPoints.at(j) == controlPoints.at(j + 1)) {
		}
	}

	return std::vector<vec2<unsigned>>();
}

std::vector<vec2<unsigned>> Curve::generateSliderPoints(std::vector<vec2<unsigned>> controlPoints, unsigned numPoints)
{
	return std::vector<vec2<unsigned>>();
	//if 
	return std::vector<vec2<unsigned>>();
}
