#include "Input.h"

void Input::sendKeyInput(char key, bool pressed) {
	INPUT keyboardInput = { 0 };
	keyboardInput.type = INPUT_KEYBOARD;
	keyboardInput.ki.wVk = VkKeyScanEx(key, GetKeyboardLayout(0)); // Map char to keycode
	keyboardInput.ki.dwFlags = pressed ? 0 : KEYEVENTF_KEYUP;
	keyboardInput.ki.time = 0;
	SendInput(1, &keyboardInput, sizeof(keyboardInput));
}

void Input::moveMouseInstant(vec2<double> point) {
	INPUT mouseInput;// = { 0 };
	mouseInput.type = INPUT_MOUSE;
	mouseInput.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE; // Absolute position for x/y
																	 // Map to Windows coordinate system (defined as (0,0) to (65535,65535), (TL to BR))
	mouseInput.mi.dx = point.getX() * (65535 / 1920);
	mouseInput.mi.dy = point.getY() * (65535 / 1080);
	mouseInput.mi.time = 0;
	SendInput(1, &mouseInput, sizeof(mouseInput));
}