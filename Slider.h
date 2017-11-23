#pragma once
#include "HitObject.h"
#include "vec2.h"
#include <vector>

class Slider :
	public HitObject
{
public:
	Slider(int x, int y, unsigned startTime, unsigned endTime, unsigned type,
		 wchar_t sliderType, std::vector<vec2<int>> controlPoints);
	~Slider();

	virtual void mouseMovement(HANDLE osuProcess, DWORD timeAddress);

	void printInfo() const;

private:
	wchar_t sliderType;
	std::vector<vec2<int>> controlPoints;
	unsigned repeat;
	double pixelLength;
	std::vector<vec2<int>> sliderPoints;
};

