#pragma once
#include "HitObject.h"

class Spinner :
	public HitObject
{
public:
	Spinner(double x, double y, int startTime, int endTime, unsigned type);
	~Spinner();

	virtual void mouseMovement(HANDLE osuProcess, DWORD timeAddress);

	void printInfo() const;

};

