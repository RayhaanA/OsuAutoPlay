#pragma once
#include "HitObject.h"
#include "TimingPoint.h"
#include <vector>
#include <string>
#include <memory>

class Beatmap
{
private:
	std::string songFilePath;

	// Needed for input timing as period for
	// a "perfectly" timed hit decreases as 
	// overall difficulty increases
	unsigned overallDifficulty;
	double sliderMultiplier;

	// Wide string for unicode support
	std::wstring songTitle;
	std::wstring songArtist;


	std::vector<TimingPoint> timingPoints;
	// Pointer of base type to avoid object slicing
	std::vector<std::unique_ptr<HitObject>> hitObjects;

public:
	Beatmap(std::string filePath);
	~Beatmap();

	bool readSongFile();

	void printBeatmap() const;
};
