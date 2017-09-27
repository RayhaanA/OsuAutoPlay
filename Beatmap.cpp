#include "Beatmap.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include "Spinner.h"
#include "Slider.h"
#include "HitCircle.h"

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

	// (TODO: Could create function to split line by delimiter
	//  instead and access required positions by known index from
	//  osu! documentation)

	while (std::getline(beatmapFile, line)) {
		// Find metadata
		if (!foundMetaData) {
			if (line.find(L"TitleUnicode:") != std::string::npos) {
				songTitle = getLineValue(line);
			}
			else if (line.find(L"ArtistUnicode:") != std::string::npos) {
				songArtist = getLineValue(line);
				foundMetaData = true;
			}
			continue;
		}

		// Find OD
		if (!foundDiffculty) {
			if (line.find(L"OverallDifficulty") != std::string::npos) {
				overallDifficulty = std::stoul(getLineValue(line));
			}
			else if (line.find(L"SliderMultiplier") != std::string::npos) {
				sliderMultiplier = std::stod(getLineValue(line));
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

					pos = line.find(L",");
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
					hitObjects.push_back(parseHitObject(line));
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

void Beatmap::parseHitObject(std::wstring line) {
	unsigned x;
	unsigned y; 
	unsigned startTime;
	unsigned endTime;
	HitObject::types type = HitObject::types::EMPTY;

	std::vector<std::wstring> lineComponents = splitLine(line);
	x = std::stoul(lineComponents.at(0));
	y = std::stoul(lineComponents.at(1));
	startTime = std::stoul(lineComponents.at(2));
	type = static_cast<HitObject::types>(std::stoi(lineComponents.at(3)));

	if (type == HitObject::types::SLIDER) {
		endTime = std::stoul(lineComponents.at(5));
		std::shared_ptr<Spinner> s(new Spinner(x, y, startTime, endTime, type));
		hitObjects.push_back(s);
	}
	else if (type == HitObject::types::CIRCLE) {
		endTime = startTime + 2;
		std::shared_ptr<HitCircle> c(new HitCircle(x, y, startTime, endTime, type));
		hitObjects.push_back(c);
	}
	else if (type == HitObject::types::SLIDER) {

	}

	
}

std::wstring Beatmap::getLineValue(std::wstring line) {
	return (line.substr(line.find(L":") + 1));
}

std::vector<std::wstring> Beatmap::splitLine(std::wstring line) {
	std::vector<std::wstring> lineComponents;
	while (line.find(L",") != std::string::npos) {
		unsigned pos = line.find(L",");
		lineComponents.push_back(line.substr(0, pos));
		line.erase(0, pos + 1);
	}
	return lineComponents;
}