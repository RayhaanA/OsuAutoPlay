#include "Beatmap.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include "Spinner.h"
#include "Slider.h"
#include "HitCircle.h"
#include <WinUser.h>

const double X_SCREEN_RES = 1920.0;
const double Y_SCREEN_RES = 1080.0;

// Eventually read these two values from config file
const double OSU_X_SCREEN_RES = 1920.0;
const double OSU_Y_SCREEN_RES = 1080.0;

// osu! hit object locations initially correspond to a 640x480 window and are scaled to
// different resolutions
const double X_SCALE_FACTOR = 3.0;
const double Y_SCALE_FACTOR = 2.25;

const double X_OFFSET = 192.0;
const double Y_OFFSET = 108.0;

Beatmap::Beatmap(std::wstring filePath) : songFilePath(filePath) {}


Beatmap::~Beatmap() {
}

bool Beatmap::readSongFile() {
	// Time file reading time for debugging purposes
	auto start = std::chrono::high_resolution_clock::now();

	std::wifstream beatmapFile(songFilePath);

	if (!beatmapFile) {
		std::wcerr << L"Failed to open beatmap file! Please check file path!" << std::endl;
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
			if (line.find(L"Title:") != std::string::npos) {
				songTitle = getLineValue(line);
			}
			else if (line.find(L"Artist:") != std::string::npos) {
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
					int offset = std::stoul(timingPointInfo[0]);
					double msPerBeat = std::stod(timingPointInfo[1]);

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
				foundHitObjects = true;
			}
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

	std::vector<std::wstring> lineComponents = splitLine(line, ',');
	double x = std::stod(lineComponents[0]);
	double y = std::stod(lineComponents[1]);

	// Convert to Windows coordinates
	x = ((x * X_SCALE_FACTOR) + X_OFFSET);
	y = ((y * Y_SCALE_FACTOR) + Y_OFFSET);

	unsigned startTime = std::stoul(lineComponents[2]);
	unsigned type = std::stoi(lineComponents[3]);

	// Extract  type by masking with the bits corresponding to the hit objects
	// as defined by .osu file format. (bit 0 + bit 1 + bit 3 = 11 = 0x0B)
	type &= 0x0B;
	type = static_cast<HitObject::types>(type);

	if (type & HitObject::types::SPINNER) {
		endTime = std::stoul(lineComponents[5]);
		hitObjects.push_back(std::shared_ptr<Spinner>(new Spinner(x, y, startTime, endTime, type)));
	}
	else if (type & HitObject::types::CIRCLE) {
		endTime = startTime + 2; // Need to register input 
		hitObjects.push_back(std::shared_ptr<HitCircle>(new HitCircle(x, y, startTime, endTime, type)));
	}
	else if (type & HitObject::types::SLIDER) {
		wchar_t sliderType = lineComponents[5][0];
		double pixelLength = std::stod(lineComponents[7]);
		unsigned repeat = std::stoul(lineComponents[6]);

		TimingPoint activeTimingPoint = getActiveTimingPoint(startTime, timingPoints);

		// Calculation for roughly how long a slider will last by adding the number of repeats
		// times the pixel length of the slider divided by the pixels per beat value to 
		// the start time (100 * vel is the effective velocity for the timing point), to get the number
		// of beats it lasts for, and then multiplying by the msPerBeat of the timing point to get the length
		endTime = static_cast<unsigned>(startTime + (((repeat * pixelLength ) / \
													(100 * activeTimingPoint.getVelocity() * sliderMultiplier)) *
													activeTimingPoint.getMsPerBeat()));

		std::vector<vec2<double>> controlPoints = { vec2<double>{ x, y } };
		std::vector<std::wstring> sControlPoints = splitLine(lineComponents[5], '|');

		for (auto it = sControlPoints.begin() + 1, end = sControlPoints.end(); it != end; std::advance(it, 1)) {
			unsigned pos = it->find(':');
			// Convert control points to Windows coordinates
			vec2<double> point = { ((std::stod(it->substr(0, pos)) * X_SCALE_FACTOR) + X_OFFSET), 
									((std::stod(it->substr(pos + 1, it->length())) * Y_SCALE_FACTOR) + Y_OFFSET)
								};
			controlPoints.push_back(point);
		}

		hitObjects.push_back(std::shared_ptr<Slider>(new Slider(x, y, startTime, endTime, type, sliderType, controlPoints, repeat)));
	}	
}

std::wstring Beatmap::getLineValue(std::wstring line) {
	return (line.substr(line.find(':') + 1));
}

std::vector<std::wstring> Beatmap::splitLine(std::wstring line, const wchar_t & delimiter) {
	std::vector<std::wstring> lineComponents;

	while (line.find(delimiter) != std::string::npos) {
		unsigned pos = line.find(delimiter);
		lineComponents.push_back(line.substr(0, pos));
		line.erase(0, pos + 1);
	}

	if (lineComponents.size() < 8)
		lineComponents.push_back(line.substr(0));

	return lineComponents;
}

TimingPoint Beatmap::getActiveTimingPoint(int offset, std::vector<TimingPoint> points) {
	for (auto it = points.rbegin(), end = points.rend(); it != end; std::advance(it, 1)) {
		if ((*it).getOffset() <= offset)
			return *it;
	}

	return points[0];
}

unsigned Beatmap::getOverallDifficulty() const {
	return overallDifficulty;
}

void Beatmap::printBeatmap() const {
	std::wcout << L"Song file path: " << songFilePath << std::endl;
	std::wcout << L"Song Title: " << songTitle << std::endl;
	std::wcout << L"Song Artist: " << songArtist << std::endl;
	std::wcout << L"Overall Song Difficulty: " << overallDifficulty << std::endl;
	std::wcout << L"Slider Multipler: " << sliderMultiplier << "\n" << std::endl;

	std::wcout << L"[Timing Points]" << std::endl;
	for (const auto & i : timingPoints) {
		std::wcout << i << std::endl;
	}
	std::wcout << std::endl;
	std::wcout << L"[Hit Objects]" << std::endl;
	for (const auto & i : hitObjects) {
		std::wcout << i->getPosition().getX() << ", " << i->getPosition().getY() ;
		std::wcout << std::endl;
	}

	std::wcout << L"Number of timing points: " << timingPoints.size() << std::endl;
	std::wcout << L"Number of hit objects: " << hitObjects.size() << "\n" << std::endl;
}


