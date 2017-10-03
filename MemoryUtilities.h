#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

namespace MemoryUtilities {
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
}

