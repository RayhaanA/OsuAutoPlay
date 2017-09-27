#pragma once

class TimingPoint
{
private:
	// Inherited timing points need a reference
	static double parentMsPerBeat;
	
	unsigned offset;
	double msPerBeat;
	bool inherited;
	
public:
	TimingPoint(unsigned offset, double msPerBeat)
		: offset(offset), msPerBeat(msPerBeat), inherited(msPerBeat < 0 ? true : false) {}

	~TimingPoint() {}

	unsigned getOffset() const {
		return offset;
	}

	double getMsPerBeat() const {
		return msPerBeat;
	}

	double getParentMsPerBeat() const {
		return parentMsPerBeat;
	}

	bool getInherited() const {
		return inherited;
	}
};
