#pragma once

// Hit object parent class
// Class members and child member requirements derived from
// .osu file format page from osu! wiki
// https://osu.ppy.sh/help/wiki/osu!_File_Formats/Osu_(file_format)

class HitObject
{
public:
	HitObject(unsigned x, unsigned y, unsigned startTime, unsigned endTime, unsigned type) 
		: x(x), y(y), startTime(startTime), endTime(endTime), type(static_cast<types>(type)) {}
	~HitObject() {}

	unsigned getX() const {
		return x;
	}

	unsigned getY() const {
		return y;
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

	// Method for how the HitObject will be handled 
	// differs for all HitObject types
	// This method is for performing mouse movement and
	// clicking the object
	virtual void hit() = 0;

	// Different HitObject types defined by .osu file format
	enum types {
		EMPTY = 0,
		CIRCLE = 1,
		SLIDER = 2,
		SPINNER = 8
	};

	virtual void printInfo() const = 0;

private:
	unsigned x;
	unsigned y;
	unsigned type;
	unsigned startTime;
	unsigned endTime;
};

