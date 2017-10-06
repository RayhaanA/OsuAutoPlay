#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <limits>

namespace MemoryUtilities {
	// The following signature performs the following ops (observed using Cheat Engine)
	// DB 5D E8 - fistp dword ptr[ebp - 18] --> moves and pops the fpu register stack (ST0) to [ebp-18] as a 32-bit int
	// 8B 45 E8 - mov eax, [ebp - 18] --> moves that value to eax (the value is the in-game time)
	// A3 BC5D7705 - mov[05775DBC], eax --> moves the time value to the game's time variable
	// where BC5D7705 is the address of the time variable at the time of running the program when this was checked
	// note: address flipped due to little endian architecture ie. byte that comes first (05) actually is least significant
	const unsigned char TIME_SIGNATURE[] = "\xDB\x5D\xE8\x8B\x45\xE8\xA3";

	DWORD getOsuProcessID() {
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
			return(false);
		}

		CloseHandle(processList);
		std::wprintf(L"osu! process ID found at: %d\n", osuProcessID);
		return osuProcessID;
	}

	// Finds a pattern of byte values given a process to read from
	DWORD findPattern(HANDLE process, unsigned char pattern[]) {
		const size_t patternSize = sizeof(pattern);
		const size_t readSize = 4096; // 4KB at a time (NTFS)

		// Byte array
		unsigned char bucket[readSize];

		for (size_t i = 0; i < std::numeric_limits<int>::max(); i += readSize - patternSize) {
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
	DWORD findAndGetTimeAddress(HANDLE gameProcess) {

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


	unsigned getElapsedSongTime(HANDLE gameProcess, DWORD timeAddress, unsigned & elapsed) {
		if (!ReadProcessMemory(gameProcess, LPCVOID(timeAddress), &elapsed, sizeof(unsigned), nullptr)) {
			std::wcerr << L"Couldn't retrieve elapsed time from the time address!" << std::endl;
			return false;
		}
	}
}

