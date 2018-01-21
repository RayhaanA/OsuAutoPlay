#pragma once
#include "vec2.h"
#include <Windows.h>
#include <iostream>


namespace Input {

	void sendKeyInput(char key, bool pressed);

	// TODO: Convert osu! pixel to screen pixel accordingly
	void moveMouseInstant(vec2<double> point);

}

