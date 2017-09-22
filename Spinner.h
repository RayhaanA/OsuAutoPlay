#pragma once
#include "HitObject.h"
class Spinner :
	public HitObject
{
public:
	Spinner(unsigned x, unsigned y, unsigned startTime, unsigned endTime, unsigned type);
	~Spinner();

	void hit();
};

