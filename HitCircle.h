#pragma once
#include "HitObject.h"

class HitCircle :
	public HitObject
{
public:
	HitCircle(unsigned x, unsigned y, unsigned startTime, unsigned type, unsigned endTime);
	~HitCircle();

	void hit();

	void printInfo() const;

};