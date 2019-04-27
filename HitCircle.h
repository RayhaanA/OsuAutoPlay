#pragma once
#include "HitObject.h"

class HitCircle :
	public HitObject
{
public:
	HitCircle(double x, double y, int startTime, int endTime, unsigned type);
	~HitCircle();

	virtual void mouseMovement(HANDLE osuProcess, DWORD timeAddress);

	void printInfo() const;

};