#pragma once
#include <cmath>
#include <vector>
#include <iostream>

class TimingPoint
{
private:
	int offset;
	double msPerBeat;
	bool inherited;

	// A movement/timing modifying ratio
	// Inherited points modify slider velocity by (-1*100/msPerBeat) as the scaling ratio
	// where msPerBeat is a negative value indicating an inherited timing point
	double velocity;
	
public:
	TimingPoint(int offset, double indivMSPB)
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

	int getOffset() const {
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

	friend std::wostream& operator<<(std::wostream& os, const TimingPoint & point) {
		os << "(" << point.offset << ", " << point.msPerBeat << ", " << point.inherited
			<< ", " << point.velocity << ")";
		return os;
	}
};
