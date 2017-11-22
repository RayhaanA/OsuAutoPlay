#pragma once
#include "HitObject.h"
#include <iostream>

class Spinner :
	public HitObject
{
public:
	Spinner(int x, int y, unsigned startTime, unsigned endTime, unsigned type);
	~Spinner();

	virtual void mouseMovement(DWORD timeAddress);

	void printInfo() const;

};

