#pragma once
#include "Beatmap.h"
#include "MemoryUtilities.h"
#include "Input.h"
#include <thread>
#include <Windows.h>

namespace Modes {
	void HitOnly(size_t i, Beatmap b, HANDLE osuProcess, DWORD timeAddress, unsigned elapsed, const unsigned TOLERANCE) {
		bool keyPressed = false;

		while (i < b.hitObjects.size()) {
			auto currentObject = b.hitObjects[i];
			unsigned nextStartTime = currentObject->getStartTime();

			MemoryUtilities::getElapsedSongTime(osuProcess, timeAddress, elapsed);

			if (elapsed >= nextStartTime - TOLERANCE && !keyPressed) {
				Input::sendKeyInput('a', true);
				keyPressed = true;
			}

			if (elapsed > currentObject->getEndTime() && keyPressed) {
				Input::sendKeyInput('a', false);
				keyPressed = false;
				i++;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	void FullAuto() {} // Mouse movement and click

	void MouseMoveOnly(size_t i, Beatmap b, HANDLE osuProcess, DWORD timeAddress, unsigned elapsed, const unsigned TOLERANCE) {
		// TODO: Slider and spinner movements

		bool mouseOnObject = false;

		while (i < b.hitObjects.size()) {
			auto currentObject = b.hitObjects[i];
			unsigned nextStartTime = currentObject->getStartTime();
			std::cout << "Current start time : " << nextStartTime << " Current Time : " << elapsed <<  " " << i << "\n";
			MemoryUtilities::getElapsedSongTime(osuProcess, timeAddress, elapsed);

			if (elapsed >= nextStartTime - TOLERANCE && !mouseOnObject) {
				currentObject->mouseMovement(osuProcess, timeAddress);
				mouseOnObject = true;
			}

			if (elapsed > currentObject->getEndTime() && mouseOnObject) {
				mouseOnObject = false;
				i++;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	void RelaxMode() {} // Like hit-only but is lenient towards manual mouse aiming
}