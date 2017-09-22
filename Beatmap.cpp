#include "Beatmap.h"
#include <fstream>
#include <iostream>
#include <chrono>

Beatmap::Beatmap(std::string filePath) : songFilePath(filePath) {}


Beatmap::~Beatmap() {
}

bool Beatmap::readSongFile() {
	// Time file reading time for debugging purposes
	auto start = std::chrono::high_resolution_clock::now();

	std::ifstream beatmapFile(songFilePath);

	if (!beatmapFile) {
		std::cerr << "Failed to open beatmap file! Please check file path!" << std::endl;
		return false;
	}

	// Convenience
	typedef std::string::npos npos;

	// Buffer for getline
	std::string line;

	// Checkpoints for parsing the file
	bool foundMetaData = false;
	bool foundDiffculty = false;
	bool foundTimingPoints = false;
	bool foundHitObjects = false;

	// In terms of the relevant data we're looking for, the .osu
	// file format starts with metadata, then defines timing points, 
	// and finally hit objects 

	// (TODO: Currently using continue to prevent certain checks,
	// could use expression short-circuiting with checkpoint bools
	// instead)

	while (std::getline(beatmapFile, line)) {
		// Find metadata
		if (!foundMetaData) {
			if (line.find("TitleUnicode:") != npos) {
				songTitle = static_cast<std::wstring>(line.substr(line.find(":") + 1));
			}
			else if (line.find("ArtistUnicode:") != npos) {
				songArtist = static_cast<std::wstring>(line.substr(line.find(":") + 1));
				foundMetaData = true;
			}
			continue;
		}

		// Find OD
		if (!foundDiffculty) {
			if (line.find("OverallDifficulty") != npos) {
				overallDifficulty = std::stoul((line.substr(line.find(":") + 1)));
			}
			else if (line.find("SliderMultiplier") != npos {
				sliderMultiplier = std::stod((line.substr(line.find(":") + 1)));
				foundDiffculty = true;
			}
			continue;
		}

		// Find timing points
		if (!foundTimingPoints) {
			if (line.find("[TimingPoints]") != npos) {
				while (std::getline(beatmapFile, line) && !line.empty()) {
					unsigned pos = line.find(",");
					unsigned offset = std::stoul((line.substr(0, pos)));
					line.erase(0, pos);
					double msPerBeat = std::stod(line.substr(0, pos));
					timingPoints.push_back(TimingPoint(offset, msPerBeat));
				}
				foundTimingPoints = true;
			}
			continue;
		}

		// Find hit objects
		if (!foundHitObjects) {
			if (line.find("[HitObjects]") != npos) {
				while (std::getline(beatmapFile, line) && !line.empty()) {
					unsigned pos = line.find(",");
				}
			}
			foundHitObjects = true;
		}
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Parsing beatmap: " << songTitle << " took " << elapsed << "ms" << std::endl;

	if (!foundDiffculty || !foundHitObjects || !foundMetaData || !foundTimingPoints)
		return false;
	else
		return true;
}
