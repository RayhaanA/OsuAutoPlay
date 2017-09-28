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
					std::vector<std::wstring> timingPointInfo = splitLine(line, ',');
					unsigned offset = std::stoul(timingPointInfo.at(0));
					double msPerBeat = std::stod(timingPointInfo.at(1));

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
					parseHitObject(line);
				}
			}
			foundHitObjects = true;
		}
		// Hit objects are last section of .osu file
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
	unsigned endTime;
	HitObject::types type = HitObject::types::EMPTY;

	std::vector<std::wstring> lineComponents = splitLine(line, ',');
	unsigned x = std::stoul(lineComponents.at(0));
	unsigned y = std::stoul(lineComponents.at(1));
	unsigned startTime = std::stoul(lineComponents.at(2));
	type = static_cast<HitObject::types>(std::stoi(lineComponents.at(3)));

	if (type == HitObject::types::SPINNER) {
		endTime = std::stoul(lineComponents.at(5));
		// std::unique_ptr<Spinner>(new Spinner(x, y, startTime, endTime, type));
		hitObjects.push_back(std::unique_ptr<Spinner>(new Spinner(x, y, startTime, endTime, type)));
	}
	else if (type == HitObject::types::CIRCLE) {
		endTime = startTime + 2; // Guess
		hitObjects.push_back(std::unique_ptr<HitCircle>(new HitCircle(x, y, startTime, endTime, type)));
	}
	else if (type == HitObject::types::SLIDER) {
		wchar_t sliderType = lineComponents.at(5)[0];
		double pixelLength = std::stod(lineComponents.at(7));
		unsigned repeat = std::stoul(lineComponents.at(6));

		TimingPoint activeTimingPoint = TimingPoint::getActiveTimingPoint(startTime, timingPoints);

		// Calculation for roughly how long a slider will last by adding the number of repeats
		// times the pixel length of the slider divided by the pixels per beat value to 
		// the start time (100 * vel is the effective velocity for the timing point)
		unsigned endTime = static_cast<unsigned>(std::ceil(startTime + repeat * pixelLength 
											/ (100 * activeTimingPoint.getVelocity() * sliderMultiplier)));
		std::vector<vec2<unsigned>> controlPoints;
		std::vector<std::wstring> sControlPoints = splitLine(lineComponents.at(5), '|');
		for (auto & i : sControlPoints) {
			unsigned pos = i.find(':');
			vec2<unsigned> point = { std::stoul(i.substr(0, pos)), std::stoul(i.substr(pos + 1, i.length())) };
			controlPoints.push_back(point);
		}

		hitObjects.push_back(std::unique_ptr<Slider>(new Slider(x, y, startTime, endTime, type, sliderType, controlPoints)));
	}	
}

std::wstring Beatmap::getLineValue(std::wstring line) {
	return (line.substr(line.find(',') + 1));
}

std::vector<std::wstring> Beatmap::splitLine(std::wstring line, const wchar_t & delimiter) {
	std::vector<std::wstring> lineComponents;
	while (line.find(delimiter) != std::string::npos) {
		unsigned pos = line.find(delimiter);
		lineComponents.push_back(line.substr(0, pos));
		line.erase(0, pos + 1);
	}
	return lineComponents;
}

void Beatmap::printBeatmap() const {
	std::cout << "Song file path: " << songFilePath << std::endl;
	std::wcout << L"Song Title: " << songTitle << std::endl;
	std::wcout << L"Song Artist: " << songArtist << std::endl;
	std::cout << "Overall Song Difficulty: " << overallDifficulty << std::endl;
	std::cout << "Slider Multipler: " << sliderMultiplier << "\n" << std::endl;

}


