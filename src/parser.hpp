#pragma once
#include <string>

namespace Parser {
	bool toGameBytes(std::string& fileDataIn, unsigned char* dummyDataOut, size_t* sizeOut, bool reversalTiming = false);
	std::string toFile(unsigned char* dummyData, size_t size);

	std::string getErrorMessage();
}