#include "game.hpp"

#include <Windows.h>
#include <Psapi.h>
#include <string>
#include <sstream>
#include <fstream>

#include "definitions.hpp"
#include "ui.hpp"
#include "parser.hpp"

namespace Game {
	static HANDLE gameProcess = NULL;
	static DWORD baseAddress = NULL;

	HANDLE getGameProcess();
	DWORD getGameBaseAddress(HANDLE gameProcess);
}

bool Game::initialize() {
	gameProcess = getGameProcess();
	if (gameProcess == NULL) {
		return false;
	}
	baseAddress = getGameBaseAddress(gameProcess);
	if (baseAddress == -1) {
		std::string errorText(128, '\0');
		sprintf_s(&errorText[0], 128, "Failed to get game base address. (0x%X)", GetLastError());
		UI::setStatusText(errorText);
		return false;
	}
	return true;
}

bool Game::isActive() {
	return (FindWindowW(GameWindowName, 0) && (gameProcess != NULL));
}

void Game::exit() {
	CloseHandle(gameProcess);
}

HANDLE Game::getGameProcess() {
	HWND gameWindow = FindWindowW(GameWindowName, NULL);
	DWORD gameProcID = 0;

	if (gameWindow == NULL) {
		return NULL;
	}

	GetWindowThreadProcessId(gameWindow, &gameProcID);
	if (gameProcID == 0) {
		std::string errorText(128, '\0');
		sprintf_s(&errorText[0], 128, "Failed to obtain game process ID. (0x%X)", GetLastError());
		UI::setStatusText(errorText.c_str());
		return NULL;
	}

	HANDLE gameProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, FALSE, gameProcID);
	if (gameProcess == NULL) {
		std::string errorText(128, '\0');
		sprintf_s(&errorText[0], 128, "Failed to open game process. (0x%X)", GetLastError());
		UI::setStatusText(errorText.c_str());
		return NULL;
	}
	return gameProcess;
}

DWORD Game::getGameBaseAddress(HANDLE gameProcess) {
	HMODULE modules[1024];

	DWORD numBytes = 0;
	if (EnumProcessModules(gameProcess, modules, sizeof(modules), &numBytes)) {
		for (unsigned int i = 0; i < (numBytes / sizeof(HMODULE)); i++) {
			wchar_t moduleName[MAX_PATH];
			if (GetModuleFileNameEx(gameProcess, modules[i], moduleName, sizeof(moduleName) / sizeof(wchar_t))) {
				std::wstring moduleNameW = moduleName;
				if (moduleNameW.find(GameExecutableName) != std::wstring::npos) {
					return (DWORD)modules[i];
				}
			}
		}
	}
	return -1;
}

void Game::readMemory(DWORD address, void* data, size_t size) {
	ReadProcessMemory(Game::gameProcess, (void*)(Game::baseAddress + address), data, size, 0);
}

void Game::writeMemory(DWORD address, void* data, size_t size) {
	WriteProcessMemory(Game::gameProcess, (void*)(Game::baseAddress + address), data, size, 0);
}

size_t Game::getDummyDataSize(unsigned int slot) {
	unsigned int frames = 0;
	switch (slot) {
	case 1:	readMemory(DummyData1_SizeAddress, &frames, sizeof(frames)); break;
	case 2:	readMemory(DummyData2_SizeAddress, &frames, sizeof(frames)); break;
	case 3:	readMemory(DummyData3_SizeAddress, &frames, sizeof(frames)); break;
	case 4:	readMemory(DummyData4_SizeAddress, &frames, sizeof(frames)); break;
	case 5:	readMemory(DummyData5_SizeAddress, &frames, sizeof(frames)); break;
	default:
		return 0;
	}
	return frames;
}

bool Game::importDummyData(std::string path, unsigned int slot, bool reversalTiming, int playerSlot) {
	std::ifstream file(path.c_str());
	std::stringstream ss;
	std::string buffer;
	std::getline(file, buffer);
	while (file.good()) {
		ss << buffer << "\n";
		std::getline(file, buffer);
	}
	buffer = ss.str();

	unsigned char dummyData[DummyDataMaxSize];
	memset(dummyData, 0, DummyDataMaxSize);

	size_t dummySize;
	if (!Parser::toGameBytes(buffer, dummyData, &dummySize, reversalTiming)) {
		return false;
	}

	clearDummyData(slot);

	unsigned int enabledFlag = ((0x3333U) * (slot - 1));

	if (playerSlot == 0) {
		switch (slot) {
		case 1:
			writeMemory(DummyData1_SizeAddress, &dummySize, sizeof(int));
			writeMemory(DummyData1_BlockStartAddress_P1, (void*)dummyData, dummySize);
			writeMemory(DummyData1_EnabledFlag, &enabledFlag, sizeof(int));
			break;
		case 2:
			writeMemory(DummyData2_SizeAddress, &dummySize, sizeof(int));
			writeMemory(DummyData2_BlockStartAddress_P1, (void*)dummyData, dummySize);
			writeMemory(DummyData2_EnabledFlag, &enabledFlag, sizeof(int));
			break;
		case 3:
			writeMemory(DummyData3_SizeAddress, &dummySize, sizeof(int));
			writeMemory(DummyData3_BlockStartAddress_P1, (void*)dummyData, dummySize);
			writeMemory(DummyData3_EnabledFlag, &enabledFlag, sizeof(int));
			break;
		case 4:
			writeMemory(DummyData4_SizeAddress, &dummySize, sizeof(int));
			writeMemory(DummyData4_BlockStartAddress_P1, (void*)dummyData, dummySize);
			writeMemory(DummyData4_EnabledFlag, &enabledFlag, sizeof(int));
			break;
		case 5:
			writeMemory(DummyData5_SizeAddress, &dummySize, sizeof(int));
			writeMemory(DummyData5_BlockStartAddress_P1, (void*)dummyData, dummySize);
			writeMemory(DummyData5_EnabledFlag, &enabledFlag, sizeof(int));
			break;
		default:
			return false;
		}
	}
	if (playerSlot == 1) {
		switch (slot) {
		case 1:
			writeMemory(DummyData1_SizeAddress, &dummySize, sizeof(int));
			writeMemory(DummyData1_BlockStartAddress, (void*)dummyData, dummySize);
			writeMemory(DummyData1_EnabledFlag, &enabledFlag, sizeof(int));
			break;
		case 2:
			writeMemory(DummyData2_SizeAddress, &dummySize, sizeof(int));
			writeMemory(DummyData2_BlockStartAddress, (void*)dummyData, dummySize);
			writeMemory(DummyData2_EnabledFlag, &enabledFlag, sizeof(int));
			break;
		case 3:
			writeMemory(DummyData3_SizeAddress, &dummySize, sizeof(int));
			writeMemory(DummyData3_BlockStartAddress, (void*)dummyData, dummySize);
			writeMemory(DummyData3_EnabledFlag, &enabledFlag, sizeof(int));
			break;
		case 4:
			writeMemory(DummyData4_SizeAddress, &dummySize, sizeof(int));
			writeMemory(DummyData4_BlockStartAddress, (void*)dummyData, dummySize);
			writeMemory(DummyData4_EnabledFlag, &enabledFlag, sizeof(int));
			break;
		case 5:
			writeMemory(DummyData5_SizeAddress, &dummySize, sizeof(int));
			writeMemory(DummyData5_BlockStartAddress, (void*)dummyData, dummySize);
			writeMemory(DummyData5_EnabledFlag, &enabledFlag, sizeof(int));
			break;
		default:
			return false;
		}
	}

	return true;
}

bool Game::exportDummyData(unsigned int slot, std::string path, int playerSlot) {
	size_t dummySize = getDummyDataSize(slot);
	unsigned char dummyData[DummyDataMaxSize];
	memset(dummyData, 0, sizeof(dummyData));

	if (playerSlot == 0) {
		switch (slot) {
		case 1: readMemory(DummyData1_BlockStartAddress_P1, &dummyData, dummySize); break;
		case 2: readMemory(DummyData2_BlockStartAddress_P1, &dummyData, dummySize); break;
		case 3: readMemory(DummyData3_BlockStartAddress_P1, &dummyData, dummySize); break;
		case 4: readMemory(DummyData4_BlockStartAddress_P1, &dummyData, dummySize); break;
		case 5: readMemory(DummyData5_BlockStartAddress_P1, &dummyData, dummySize); break;
		default:
			return false;
		}
	}
	if (playerSlot == 1) {
		switch (slot) {
		case 1: readMemory(DummyData1_BlockStartAddress, &dummyData, dummySize); break;
		case 2: readMemory(DummyData2_BlockStartAddress, &dummyData, dummySize); break;
		case 3: readMemory(DummyData3_BlockStartAddress, &dummyData, dummySize); break;
		case 4: readMemory(DummyData4_BlockStartAddress, &dummyData, dummySize); break;
		case 5: readMemory(DummyData5_BlockStartAddress, &dummyData, dummySize); break;
		default:
			return false;
		}
	}


	std::string buffer = Parser::toFile(dummyData, dummySize);

	std::ofstream file(path.c_str(), std::ofstream::out | std::ofstream::trunc);
	file << buffer.c_str();
	file.close();

	return true;
}

bool Game::clearDummyData(unsigned int slot) {
	unsigned char dummyData[DummyDataMaxSize];
	memset(dummyData, 0, sizeof(dummyData));
	unsigned int size = 0;
	switch (slot) {
	case 1:
		writeMemory(DummyData1_BlockStartAddress, &dummyData, DummyDataMaxSize);
		writeMemory(DummyData1_BlockStartAddress_P1, &dummyData, DummyDataMaxSize);
		writeMemory(DummyData1_SizeAddress, &size, sizeof(int));
		writeMemory(DummyData1_EnabledFlag, &size, sizeof(int));
		break;
	case 2:
		writeMemory(DummyData2_BlockStartAddress, &dummyData, DummyDataMaxSize);
		writeMemory(DummyData2_BlockStartAddress_P1, &dummyData, DummyDataMaxSize);
		writeMemory(DummyData2_SizeAddress, &size, sizeof(int));
		writeMemory(DummyData2_EnabledFlag, &size, sizeof(int));
		break;
	case 3:
		writeMemory(DummyData3_BlockStartAddress, &dummyData, DummyDataMaxSize);
		writeMemory(DummyData3_BlockStartAddress_P1, &dummyData, DummyDataMaxSize);
		writeMemory(DummyData3_SizeAddress, &size, sizeof(int));
		writeMemory(DummyData3_EnabledFlag, &size, sizeof(int));
		break;
	case 4:
		writeMemory(DummyData4_BlockStartAddress, &dummyData, DummyDataMaxSize);
		writeMemory(DummyData4_BlockStartAddress_P1, &dummyData, DummyDataMaxSize);
		writeMemory(DummyData4_SizeAddress, &size, sizeof(int));
		writeMemory(DummyData4_EnabledFlag, &size, sizeof(int));
		break;
	case 5:
		writeMemory(DummyData5_BlockStartAddress, &dummyData, DummyDataMaxSize);
		writeMemory(DummyData5_BlockStartAddress_P1, &dummyData, DummyDataMaxSize);
		writeMemory(DummyData5_SizeAddress, &size, sizeof(int));
		writeMemory(DummyData5_EnabledFlag, &size, sizeof(int));
		break;
	default:
		return false;
	}
	return true;
}
