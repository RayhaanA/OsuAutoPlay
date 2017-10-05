#pragma once
#include "HitObject.h"
#include "TimingPoint.h"
#include <vector>
#include <string>
#include <memory>

class Beatmap
{
private:
	std::wstring songFilePath;

	// Needed for input timing as period for
	// a "perfectly" timed hit decreases as 
	// overall difficulty increases
	unsigned overallDifficulty;
	double sliderMultiplier;

	// Wide string for unicode support
	std::wstring songTitle;
	std::wstring songArtist;


public:
	Beatmap(std::wstring filePath);
	~Beatmap();

	bool readSongFile();

	void printBeatmap() const;

	unsigned getOverallDifficulty() const;

	// Helpers
	void parseHitObject(std::wstring line);
	std::vector<std::wstring> splitLine(std::wstring line, const wchar_t & delimiter);
	std::wstring getLineValue(std::wstring line); // For lines with only one piece of data
												  // ie. single value after colon
												  // For finding out which timing point is active at the time a HitObject is

	// Get active timing point for slider velocity calculations
	TimingPoint getActiveTimingPoint(int offset, std::vector<TimingPoint> points);

	std::vector<TimingPoint> timingPoints;
	// Pointer of base type to avoid object slicing
	std::vector<std::shared_ptr<HitObject>> hitObjects;
};
