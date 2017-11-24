#include "MemoryUtilities.h"
#include <TlHelp32.h>
#include <iostream>
#include <limits>

DWORD MemoryUtilities::getOsuProcessID() {
	// Use TlHelp to get handle to process list
	HANDLE processList = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	if (processList == INVALID_HANDLE_VALUE) {
		std::wcerr << L"Couldn't retrive process list snapshot!" << std::endl;
		return(false);
	}

	PROCESSENTRY32 entry = {}; // Iterating through snapshot
	entry.dwSize = sizeof(PROCESSENTRY32);

	DWORD osuProcessID = {};

	if (Process32First(processList, &entry)) {
		while (Process32Next(processList, &entry)) {
			if (std::string(entry.szExeFile) == "osu!.exe") {
				osuProcessID = entry.th32ProcessID;
			}
		}
	}
	else {
		std::wcerr << L"Failed to find osu! process id!" << std::endl;
		CloseHandle(processList);
		return(false);
	}

	CloseHandle(processList);
	std::wprintf(L"osu! process ID found at: %d\n", osuProcessID);
	return osuProcessID;
}

// Finds a pattern of byte values given a process to read from
DWORD MemoryUtilities::findPattern(HANDLE process, unsigned char pattern[]) {
	const size_t patternSize = sizeof(pattern);
	const size_t readSize = 4096; // 4KB at a time (NTFS)

								  // Byte array
	unsigned char bucket[readSize];

	for (unsigned i = 1; i < (std::numeric_limits<int>::max)(); i += readSize - patternSize) {
		// Load 4KB chunk values into byte array
		ReadProcessMemory(process, (LPCVOID)i, bucket, readSize, nullptr);

		for (size_t j = 0; j < readSize; j++) {
			bool correctByte = true;
			// Iterate through all combinations of bytes checking to see if patterns match
			for (size_t k = 0; k < patternSize; k++) {
				if (bucket[j + k] != pattern[k]) {
					correctByte = false;
					break;
				}
			}
			if (correctByte) {
				return i + j; // Return bucket address value + offset within bucket as 
							  // starting point for byte pattern
			}
		}
	}
	std::wcerr << L"Couldn't find time signature pattern!" << std::endl;
	return NULL;
}

// Get the base/starting address for the time address
// Uses the findPattern function to get the location of the beginning of the
// op codes (op code DB) that are used to write into the game's time variable.
// Then, as seen in the TIME_SIGNATURE variable, the op codes are: DB, 5D, E8, 8B, 45, E8, and A3.
// As a result, incrementing by 7 bytes of data will get us to the start of the bytes after
// A3, which was found to be the start of the address of the in-game time variable
DWORD MemoryUtilities::findAndGetTimeAddress(HANDLE gameProcess) {

	DWORD timeBaseAddress = findPattern(gameProcess, PBYTE(TIME_SIGNATURE)) + 7;
	DWORD timeAddress = {};

	// Read the size of a DWORD (32-bit) into time address variable
	if (!ReadProcessMemory(gameProcess, LPCVOID(timeBaseAddress), &timeAddress, sizeof(DWORD), nullptr)) {
		std::wcerr << L"Couldn't read time address into memory!" << std::endl;
		return false;
	}
	std::wprintf(L"osu! time address found at: %x\n", timeAddress);
	return timeAddress;
}


unsigned MemoryUtilities::getElapsedSongTime(HANDLE gameProcess, DWORD timeAddress, unsigned & elapsed) {
	if (!ReadProcessMemory(gameProcess, LPCVOID(timeAddress), &elapsed, sizeof(unsigned), nullptr)) {
		std::wcerr << L"Couldn't retrieve elapsed time from the time address!" << std::endl;
		return false;
	}
}