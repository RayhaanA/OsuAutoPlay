#pragma once
#include "HitObject.h"

class Spinner :
	public HitObject
{
public:
	Spinner(int x, int y, unsigned startTime, unsigned endTime, unsigned type);
	~Spinner();

	virtual void mouseMovement(HANDLE osuProcess, DWORD timeAddress);

	void printInfo() const;

};

