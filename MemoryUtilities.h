#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <limits>

namespace MemoryUtilities {
	const unsigned char TIME_SIGNATURE[] = "";

	DWORD getOsuProcessID() {
		// Use TlHelp to get handle to process list
		HANDLE processList = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
		if (processList == INVALID_HANDLE_VALUE) {
			std::wcerr << L"Couldn't retrive process list snapshot!" << std::endl;
			return(false);
		}

		PROCESSENTRY32 entry; // Iterating through snapshot
		entry.dwSize = sizeof(PROCESSENTRY32); 

		DWORD osuProcessID;

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
		return osuProcessID;
	}


	DWORD findPattern(HANDLE process, unsigned char pattern[]) {
		const size_t patternSize = sizeof(pattern);
		const size_t readSize = 4096; // 4Kb at a time (NTFS)

		unsigned char bucket[readSize];

		for (size_t i = 0; i < std::numeric_limits<int>::max(); i += readSize - patternSize) {
			// Get bytes from chunk
			ReadProcessMemory(process, (LPCVOID)i, bucket, readSize, nullptr);

			for (size_t j = 0; j < readSize; j++) {
				bool correctByte = true;
				// Iterate through bytes checking to see if patterns match
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


	DWORD findAndGetTimeAddress(HANDLE gameProcess) {
		// Get the base/starting address for the time address
		// Add 7 because the time signature is 7 bytes long and Windows is little endian
		// which means it will provide the address that contains the beginning value
		// for the time signature
		DWORD timeBaseAddress = findPattern(gameProcess, PBYTE(TIME_SIGNATURE)) + 7;
		DWORD timeAddress;

		if (!ReadProcessMemory(gameProcess, LPCVOID(timeBaseAddress), &timeAddress, sizeof(DWORD), nullptr)) {
			std::wcerr << L"Couldn't read time address into memory!" << std::endl;
			return false;
		}

		return timeAddress;
	}


	unsigned getElapsedSongTime(HANDLE gameProcess, DWORD timeAddress) {
		unsigned elapsedTime;
		if (!ReadProcessMemory(gameProcess, LPCVOID(timeAddress), &elapsedTime, sizeof(unsigned), nullptr)) {
			std::wcerr << L"Couldn't retrieve elapsed time from the time address!" << std::endl;
			return false;
		}
		return elapsedTime;
	}
}

