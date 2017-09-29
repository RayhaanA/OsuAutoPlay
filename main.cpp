#include "vec2.h"
#include "TimingPoint.h"
#include <vector>
#include <memory>
#include "Beatmap.h"
#include <locale>

int main() {
	std::wcout.imbue(std::locale("Japanese"));

	Beatmap b("C:\\Program Files (x86)\\osu!\\Songs\\396745 Silent Siren - Routine\\Silent Siren - Routine (sukiNathan) [Easy].osu");
	std::cout << b.readSongFile();
	std::cin.get();
	return 0;
}