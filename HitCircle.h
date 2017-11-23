#pragma once
#include "HitObject.h"

class HitCircle :
	public HitObject
{
public:
	HitCircle(int x, int y, unsigned startTime, unsigned endTime, unsigned type);
	~HitCircle();

	virtual void mouseMovement(HANDLE osuProcess, DWORD timeAddress);

	void printInfo() const;

};