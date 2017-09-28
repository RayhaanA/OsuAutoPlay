#pragma once
#include "HitObject.h"
#include <iostream>

class Spinner :
	public HitObject
{
public:
	Spinner(unsigned x, unsigned y, unsigned startTime, unsigned endTime, unsigned type);
	~Spinner();

	void hit();

	void printInfo() const;
};

