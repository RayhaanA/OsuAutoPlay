#pragma once
#include <cmath>

class TimingPoint
{
private:
	unsigned offset;
	double msPerBeat;
	bool inherited;

	// A movement/timing modifying ratio
	double velocity;
	
public:
	TimingPoint(unsigned _offset, double _msPerBeat)
		: offset(_offset), msPerBeat(_msPerBeat), inherited(_msPerBeat < 0 ? true : false) {
		// Inherited timing points need a reference
		static double parentMsPerBeat;

		if (inherited) {
			// Inherited timing points affect how fast sliders will move
			velocity = std::abs(100.0 / _msPerBeat);
			// Use last non-inherited mspb
			msPerBeat = parentMsPerBeat;
		}
		// Regular timing point
		else {
			velocity = 1.0;
			parentMsPerBeat = _msPerBeat;
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
};
