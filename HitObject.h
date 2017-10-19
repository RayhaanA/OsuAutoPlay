#pragma once
#include "vec2.h"
#include <Windows.h>
// Hit object parent class
// Class members and child member requirements derived from
// .osu file format page from osu! wiki
// https://osu.ppy.sh/help/wiki/osu!_File_Formats/Osu_(file_format)

class HitObject
{
public:
	HitObject(unsigned x, unsigned y, unsigned startTime, unsigned endTime, unsigned type) 
		: position(x, y), startTime(startTime), endTime(endTime), type(static_cast<types>(type)) {}
	~HitObject() {}

	vec2<unsigned> getPosition() const {
		return position;
	}

	unsigned getStartTime() const {
		return startTime;
	}

	unsigned getEndTime() const {
		return endTime;
	}

	unsigned getType() const {
		return type;
	}

	virtual void mouseMovement(DWORD timeAddress) = 0;

	// Different HitObject types defined by .osu file format
	enum types {
		EMPTY = 0,
		CIRCLE = 1,
		SLIDER = 2,
		SPINNER = 8
	};

	virtual void printInfo() const = 0;

private:
	vec2<unsigned> position;
	unsigned type;
	unsigned startTime;
	unsigned endTime;
};

