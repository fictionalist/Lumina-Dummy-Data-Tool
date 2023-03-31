#pragma once

#include <Windows.h>
#include <string>

namespace Game {
	bool initialize();
	bool isActive();
	void exit();

	void readMemory(DWORD address, void* data, size_t size);
	void writeMemory(DWORD address, void* data, size_t size);
	size_t getDummyDataSize(unsigned int slot);

	bool importDummyData(std::string path, unsigned int slot, bool reversalTiming);
	bool exportDummyData(unsigned int slot, std::string path);
	bool clearDummyData(unsigned int slot);
}