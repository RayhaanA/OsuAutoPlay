#pragma once
#include "HitObject.h"

class HitCircle :
	public HitObject
{
public:
	HitCircle(unsigned x, unsigned y, unsigned startTime, unsigned endTime, unsigned type);
	~HitCircle();

	virtual void mouseMovement(DWORD timeAddress);

	void printInfo() const;

};