#pragma once
#include <cmath>
#include <vector>
#include <iostream>

class TimingPoint
{
private:
	unsigned offset;
	double msPerBeat;
	bool inherited;

	// A movement/timing modifying ratio
	// Inherited points modify slider velocity by (-1*100/msPerBeat) as the scaling ratio
	// where msPerBeat is a negative value indicating an inherited timing point
	double velocity;
	
public:
	TimingPoint(unsigned offset, double indivMSPB)
		: offset(offset), msPerBeat(indivMSPB), inherited(indivMSPB < 0 ? true : false) {
		// Inherited timing points need a reference
		static double parentMsPerBeat;

		if (inherited) {
			// Inherited timing points affect how fast sliders will move
			velocity = std::abs(100.0 / indivMSPB);
			// Use last non-inherited mspb
			msPerBeat = parentMsPerBeat;
		}
		// Regular timing point
		else {
			velocity = 1.0;
			parentMsPerBeat = indivMSPB;
		}
	}

	~TimingPoint() {}

	unsigned getOffset() const {
		return offset;
	}

	double getMsPerBeat() const {
		return msPerBeat;
	}

	bool getInherited() const {
		return inherited;
	}

	double getVelocity() const {
		return velocity;
	}

	// For finding out which timing point is active at the time a HitObject is
	static TimingPoint getActiveTimingPoint(unsigned offset, std::vector<TimingPoint> points) {
		for (auto it = points.rbegin(); it != points.rend(); std::advance(it, 1)) {
			if ((*it).offset <= offset)
				return *it;
		}
		return points.at(0);
	}

	friend std::wostream& operator<<(std::wostream& os, const TimingPoint & point) {
		os << "(" << point.offset << ", " << point.msPerBeat << ", " << point.inherited
			<< ", " << point.velocity << ")";
		return os;
	}
};
