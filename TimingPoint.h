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
	TimingPoint(int offset, double indivMSPB);
	~TimingPoint();

	int getOffset() const;

	double getMsPerBeat() const;

	bool getInherited() const;

	double getVelocity() const;

	friend std::wostream& operator<<(std::wostream& os, const TimingPoint & point) {
		os << "(" << point.offset << ", " << point.msPerBeat << ", " << point.inherited
			<< ", " << point.velocity << ")";
		return os;
	}
};
