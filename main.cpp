#define WIN32_LEAN_AND_MEAN
#define NOMINMAX // Annoying scope resolution issue without this

#include "Beatmap.h"
#include "TimingPoint.h"
#include "MemoryUtilities.h"
#include "Modes.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>

const unsigned int X_SCREEN_RES = GetSystemMetrics(SM_CXSCREEN);
const unsigned int Y_SCREEN_RES = GetSystemMetrics(SM_CYSCREEN);

// Eventually read these two values from config file
const unsigned int OSU_X_SCREEN_RES = 1152;
const unsigned int OSU_Y_SCREEN_RES = 864;

// Local osu! window coordinates range from 0-512 (x) and 0-384 (y)
const double X_SCALE_FACTOR = OSU_X_SCREEN_RES / 512;
const double Y_SCALE_FACTOR = OSU_Y_SCREEN_RES / 384;

// Using wmain, wstrings, wstreams, etc. due to .osu file being in unicode format

int wmain() {
	// Get time address for accessing its value
	/*DWORD processID = MemoryUtilities::getOsuProcessID();
	if (!processID) {
		std::wcerr << L"Failed to get process ID" << std::endl;
		std::wcin.get();
		return EXIT_FAILURE;
	}

	HANDLE osuProcess = OpenProcess(PROCESS_VM_READ, false, processID);
	if (osuProcess == INVALID_HANDLE_VALUE) {
		std::wcerr << L"Failed to get osu! process handle from ID" << std::endl;
		std::wcin.get();
		return EXIT_FAILURE;
	}

	DWORD timeAddress = MemoryUtilities::findAndGetTimeAddress(osuProcess);
	*/
	Beatmap b(L"C:/Users/Co-op/Downloads/524026 The Koxx - A FOOL MOON NIGHT/The Koxx - A FOOL MOON NIGHT (Astar) [Friendofox's Galaxy].osu");
	if (!b.readSongFile()) {
		std::wcerr << L"Couldn't parse beatmap file!" << std::endl;
	}
	b.printBeatmap();
	std::cin.get();
	// Use OD to get timing tolerance (79 - 6 * OD + 0.5) (Factors of (* 1.4) and (* 0.66 + 0.33) come from effects
	// of hard rock and double time modifiers, respectively. Hard rock OD effect is capped at 10)
	// Have to add 0-15ms onto calulcated value depending on in-game latency/game performance
	// For latencies below 2.0ms seems to be fine as is (maybe need 7)
	/*const unsigned TOLERANCE = std::floor((79 - 6 * std::min((b.getOverallDifficulty() * 1.4), 10.0) + 0.5)) * 0.66 + 0.33 + 7;

	// Holder for elapsed time 
	unsigned elapsed = {};
	MemoryUtilities::getElapsedSongTime(osuProcess, timeAddress, elapsed);

	// Hardcoded wait (osu! plays the song in the menu while beatmap isn't active)
	while (elapsed != 0) {
		MemoryUtilities::getElapsedSongTime(osuProcess, timeAddress, elapsed);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	// Wait for starting
	while (elapsed < b.hitObjects[0]->getStartTime()) {
		MemoryUtilities::getElapsedSongTime(osuProcess, timeAddress, elapsed);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	// Play mode
	Modes::HitOnly(b, osuProcess, timeAddress, elapsed, TOLERANCE);

	CloseHandle(osuProcess);*/
	return EXIT_SUCCESS;
}