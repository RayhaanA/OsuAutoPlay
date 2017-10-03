#pragma once
#include <Windows.h>
#include <iostream>

namespace Input {

	void sendKeyInput(char key, bool pressed) {
		INPUT keyboardInput = {0};
		keyboardInput.type = INPUT_KEYBOARD;
		keyboardInput.ki.wVk = VkKeyScanEx(key, GetKeyboardLayout(NULL)); // Map char to keycode
		keyboardInput.ki.dwFlags = pressed ? 0 : KEYEVENTF_KEYUP;
		keyboardInput.ki.time = 0;
		SendInput(1, &keyboardInput, sizeof(keyboardInput));
	}

	// TODO: Convert osu! pixel to screen pixel accordingly
	void moveMouseInstant(vec2<unsigned> point) {
		INPUT mouseInput = {0};
		mouseInput.type = INPUT_MOUSE;
		mouseInput.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE; // Absolute position for x/y
		// Map to Windows coordinate system (defined as (0,0) to (65535,65535))
		mouseInput.mi.dx = point.getX() * (65536.f / GetSystemMetrics(SM_CXSCREEN)); 
		mouseInput.mi.dy = point.getY() * (65536.f / GetSystemMetrics(SM_CYSCREEN));
		mouseInput.mi.time = 0; 
		SendInput(1, &mouseInput, sizeof(mouseInput));
	}
}

