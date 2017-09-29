#include "vec2.h"
#include "Beatmap.h"
#include "TimingPoint.h"
#include <vector>
#include <memory>

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
	std::cin.get();
	return 0;
}