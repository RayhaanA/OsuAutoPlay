#include "TimingPoint.h"

TimingPoint::TimingPoint(int offset, double indivMSPB)  
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

TimingPoint::~TimingPoint() {}

int TimingPoint::getOffset() const {
	return offset;
}

bool TimingPoint::getInherited() const {
	return inherited;
}

double TimingPoint::getMsPerBeat() const {
	return msPerBeat;
}

double TimingPoint::getVelocity() const {
	return velocity;
}

