#pragma once
#include "Beatmap.h"
#include "MemoryUtilities.h"
#include "Input.h"
#include <thread>
#include <Windows.h>

namespace Modes {
	void HitOnly(size_t i, Beatmap b, HANDLE osuProcess, DWORD timeAddress, unsigned elapsed, const unsigned TOLERANCE,
		bool keyPressed) {
		while (i < b.hitObjects.size()) {
			auto currentObject = b.hitObjects.at(i);
			unsigned nextStartTime = currentObject->getStartTime();

			MemoryUtilities::getElapsedSongTime(osuProcess, timeAddress, elapsed);

			if (elapsed >= nextStartTime - TOLERANCE && !keyPressed) {
				Input::sendKeyInput('a', true, elapsed);
				keyPressed = true;
			}

			if (elapsed > currentObject->getEndTime() && keyPressed) {
				Input::sendKeyInput('a', false, elapsed);
				keyPressed = false;
				i++;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	void FullAuto() {}

	void MouseMoveOnly() {}

	void RelaxMode() {} // Like hit-only but is lenient towards manual mouse aiming
}