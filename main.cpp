#include "vec2.h"
#include "Beatmap.h"
#include "TimingPoint.h"
#include "Input.h"

// Trying out unicode functionality 
#include <io.h>
#include <fcntl.h>

// Using wmain, wstrings, wstreams, etc. due to .osu file being in unicode format

int wmain() {
	_setmode(_fileno(stdout), _O_U16TEXT);
	Beatmap b(L"");
	if (!b.readSongFile()) {
		std::wcerr << L"Couldn't parse beatmap file!" << std::endl;
	}
	b.printBeatmap();
	Input a;
	a.sendKeyInput('a', true);
	a.sendKeyInput('b', true);
	a.sendKeyInput('c', true);
	a.sendKeyInput('d', true);
	a.sendKeyInput('e', true);
	a.sendKeyInput('a', true);
	for (unsigned i = 0; i < b.hitObjects.size(); i++) {
		a.moveMouseInstant(vec2<unsigned>{b.hitObjects.at(i)->getX(), b.hitObjects.at(i)->getY()});
		Sleep(20);
	}
	std::cin.get();
	return 0;
}