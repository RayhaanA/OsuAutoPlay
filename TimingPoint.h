#pragma once

class TimingPoint
{
private:
	unsigned offset;
	double msPerBeat;
	bool inherited;
	
public:
	TimingPoint(unsigned offset, double msPerBeat)
		: offset(offset), msPerBeat(msPerBeat), inherited(msPerBeat < 0 ? true : false) {}

	~TimingPoint(); 

	unsigned getOffset const () {
		return offset;
	}

	double getMsPerBeat const () {
		return msPerBeat;
	}

	bool getInherited const () {
		return inherited;
	}
};
