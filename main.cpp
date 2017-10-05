#define WIN32_LEAN_AND_MEAN
#define NOMINMAX // Annoying scope resolution issue without this

#include "Beatmap.h"
#include "TimingPoint.h"
#include "Input.h"
#include "MemoryUtilities.h"
#include <thread>
#include <chrono>
#include <algorithm>

// Using wmain, wstrings, wstreams, etc. due to .osu file being in unicode format

int wmain() {
	Beatmap b(L"C:/Users/Co-op/Downloads/524026 The Koxx - A FOOL MOON NIGHT/The Koxx - A FOOL MOON NIGHT (Astar) [Friendofox's Galaxy].osu");
	if (!b.readSongFile()) {
		std::wcerr << L"Couldn't parse beatmap file!" << std::endl;
	}
	//std::wcout << b.hitObjects.size();
	// Use OD to get timing tolerance (79 - 6 * OD + 0.5) (Factors of (* 1.4) and (* 0.66 + 0.33) come from effects
	// of hard rock and double time modifiers, respectively. Hard rock OD effect is capped at 10)
	const unsigned TOLERANCE = std::floor((79 - 6 * std::min((b.getOverallDifficulty() * 1.4), 10.0) + 0.5)) * 0.66 + 0.33;

	//b.printBeatmap();
	
	// Testing movement to hit objects
	auto currentObject = b.hitObjects.front();

	// Starting point
	Input::moveMouseInstant(currentObject->getPosition());

	size_t i = 1;
	auto start = std::chrono::high_resolution_clock::now();
	
	bool keyPressed = false;

	unsigned timeUntilNextHit = 0;
	unsigned elapsed;
	unsigned nextStartTime;
	vec2<unsigned> currentPosition = currentObject->getPosition();

	while (i < b.hitObjects.size()) {
		currentObject = b.hitObjects.at(i);
		elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
		nextStartTime = currentObject->getStartTime();
		timeUntilNextHit = nextStartTime - elapsed;

		/*if (elapsed <= nextStartTime && !keyPressed) {
			auto direction = currentObject->getPosition() - b.hitObjects.at(i-1)->getPosition();
			auto distance = direction.magnitude();
			auto pixelsToMove = distance / timeUntilNextHit;
			Input::moveMouseInstant((currentPosition += vec2<unsigned>{ pixelsToMove, pixelsToMove }));
		}*/

		if (elapsed >= nextStartTime - TOLERANCE && !keyPressed) {
			Input::moveMouseInstant(currentObject->getPosition());
			currentPosition = currentObject->getPosition();
			Input::sendKeyInput('.', true, i);
			keyPressed = true;
		}

		if (elapsed > currentObject->getEndTime() && keyPressed) {
			Input::sendKeyInput('.', false, i);
			keyPressed = false;
			i++;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	std::cin.get();
	return 0;
}