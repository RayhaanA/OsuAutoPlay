#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include "Beatmap.h"
#include "TimingPoint.h"
#include "Input.h"
#include "MemoryUtilities.h"
#include <thread>
#include <chrono>
#include <algorithm>

// Using wmain, wstrings, wstreams, etc. due to .osu file being in unicode format

int wmain() {
	Beatmap b(L"");
	if (!b.readSongFile()) {
		std::wcerr << L"Couldn't parse beatmap file!" << std::endl;
	}

	// Use OD to get timing tolerance (79 - 6 * OD + 0.5) (Factors of (* 1.4) and (* 0.66 + 0.33) come from effects
	// of hard rock and double time modifiers, respectively. Hard rock OD effect is capped at 10)
	const unsigned TOLERANCE = std::floor((79 - 6 * std::min((b.getOverallDifficulty() * 1.4), 10.0) + 0.5)) * 0.66 + 0.33;

	//b.printBeatmap();
	
	// Testing movement to hit objects
	size_t i = 0;
	auto start = std::chrono::high_resolution_clock::now();
	bool keyPressed = false;

	while (i < b.hitObjects.size()) {
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();

		if (elapsed >= b.hitObjects.at(i)->getStartTime() - TOLERANCE && !keyPressed) {
			Input::moveMouseInstant(b.hitObjects.at(i)->getPosition());
			Input::sendKeyInput('.', true, i);
			keyPressed = true;
		}

		if (elapsed > b.hitObjects.at(i)->getEndTime() && keyPressed) {
			Input::sendKeyInput('.', false, i);
			keyPressed = false;
			i++;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	std::cin.get();
	return 0;
}