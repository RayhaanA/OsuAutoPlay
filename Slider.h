#pragma once
#include "HitObject.h"
#include "vec2.h"
#include <vector>

class Slider :
	public HitObject
{
public:
	Slider(double x, double y, int startTime, int endTime, unsigned type,
		 wchar_t sliderType, std::vector<vec2<double>> controlPoints, unsigned repeat);
	~Slider();

	virtual void mouseMovement(HANDLE osuProcess, DWORD timeAddress);

	void printInfo() const;

private:
	wchar_t sliderType;
	std::vector<vec2<double>> controlPoints;
	unsigned repeat;
	double pixelLength;
	std::vector<vec2<double>> sliderPoints;
};

