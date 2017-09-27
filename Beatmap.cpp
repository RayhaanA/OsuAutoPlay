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

	std::wifstream beatmapFile(songFilePath);

	if (!beatmapFile) {
		std::cerr << "Failed to open beatmap file! Please check file path!" << std::endl;
		return false;
	}

	// Buffer for getline
	std::wstring line;

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
			if (line.find(L"TitleUnicode:") != std::string::npos) {
				songTitle = line.substr(line.find(L":") + 1);
			}
			else if (line.find(L"ArtistUnicode:") != std::string::npos) {
				songArtist = line.substr(line.find(L":") + 1);
				foundMetaData = true;
			}
			continue;
		}

		// Find OD
		if (!foundDiffculty) {
			if (line.find(L"OverallDifficulty") != std::string::npos) {
				overallDifficulty = std::stoul((line.substr(line.find(L":") + 1)));
			}
			else if (line.find(L"SliderMultiplier") != std::string::npos) {
				sliderMultiplier = std::stod((line.substr(line.find(L":") + 1)));
				foundDiffculty = true;
			}
			continue;
		}

		// Find timing points
		if (!foundTimingPoints) {
			if (line.find(L"TimingPoints") != std::string::npos) {
				while (std::getline(beatmapFile, line) && !line.empty()) {
					unsigned pos = line.find(L",");
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
			if (line.find(L"HitObjects") != std::string::npos) {
				while (std::getline(beatmapFile, line) && !line.empty()) {
					unsigned pos = line.find(L",");
				}
			}
			foundHitObjects = true;
		}
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::wcout << L"Parsing beatmap: " << songTitle << L" took " << elapsed.count() << L"ms" << std::endl;

	if (!foundDiffculty || !foundHitObjects || !foundMetaData || !foundTimingPoints)
		return false;
	else
		return true;
}
