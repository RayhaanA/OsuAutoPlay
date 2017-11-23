#pragma once
#include <Windows.h>

namespace MemoryUtilities {
	// The following signature performs the following ops (observed using Cheat Engine)
	// DB 5D E8 - fistp dword ptr[ebp - 18] --> moves and pops the fpu register stack (ST0) to [ebp-18] as a 32-bit int
	// 8B 45 E8 - mov eax, [ebp - 18] --> moves that value to eax (the value is the in-game time)
	// A3 BC5D7705 - mov[05775DBC], eax --> moves the time value to the game's time variable
	// where BC5D7705 is the address of the time variable at the time of running the program when this was checked
	// note: address flipped due to little endian architecture ie. byte that comes first (05) actually is least significant
	const unsigned char TIME_SIGNATURE[] = "\xDB\x5D\xE8\x8B\x45\xE8\xA3";

	DWORD getOsuProcessID();

	// Finds a pattern of byte values given a process to read from
	DWORD findPattern(HANDLE process, unsigned char pattern[]);

	// Get the base/starting address for the time address
	// Uses the findPattern function to get the location of the beginning of the
	// op codes (op code DB) that are used to write into the game's time variable.
	// Then, as seen in the TIME_SIGNATURE variable, the op codes are: DB, 5D, E8, 8B, 45, E8, and A3.
	// As a result, incrementing by 7 bytes of data will get us to the start of the bytes after
	// A3, which was found to be the start of the address of the in-game time variable
	DWORD findAndGetTimeAddress(HANDLE gameProcess);

	unsigned getElapsedSongTime(HANDLE gameProcess, DWORD timeAddress, unsigned & elapsed);
}

