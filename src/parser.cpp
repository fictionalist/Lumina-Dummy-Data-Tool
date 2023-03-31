#include "parser.hpp"
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include "definitions.hpp"

// TODO: saving from a given and loading into the same slot produces the same data, but saving from one slot and loading into another produces different data

namespace Parser {
	static std::string errorMessage(128, '\0');
}

std::string Parser::getErrorMessage() {
	return errorMessage;
}

bool Parser::toGameBytes(std::string& fileString, unsigned char* dummyData, size_t* sizeOut, bool reversalTiming) {
	*sizeOut = 0;

	std::istringstream is(fileString);
	std::string buffer;
	std::getline(is, buffer);

	std::vector<std::string> lines;

	while (is.good()) {
		lines.push_back(buffer);
		std::getline(is, buffer);
	}
	lines.push_back(buffer);

	size_t currentLine = 0;
	size_t numFrames = 0;

	size_t reversalOffset = 0;
	bool foundFirstFrame = false;

	if (reversalTiming) {
		size_t firstLineSplit = lines[0].find(":");
		if (firstLineSplit == std::string::npos) {
			sprintf_s(&errorMessage[0], 128, "Line %d: malformed frame.", currentLine);
			return false;
		}
		std::string firstLineDurationStr = lines[0].substr(0, firstLineSplit);
		std::string firstLineDurationStartStr = firstLineDurationStr.substr(0, firstLineDurationStr.find("-"));
		if (firstLineDurationStartStr.size() == 0) {
			sprintf_s(&errorMessage[0], 128, "Line %d: missing frame length.", currentLine);
			return false;
		}
		reversalOffset = atoi(firstLineDurationStartStr.c_str());
	}

	for (std::vector<std::string>::iterator line = lines.begin(); line != lines.end(); line++) {
		if (line->find("//") == 0 || line->find("#") == 0) {
			continue;
		}

		if (line->size() == 0)
			continue;

		currentLine = std::distance(lines.begin(), line);

		size_t commentPos = line->find("//");
		if (commentPos != std::string::npos) {
			*line = line->substr(0, commentPos);
		}
		commentPos = line->find("#");
		if (commentPos != std::string::npos) {
			*line = line->substr(0, commentPos);
		}

		size_t whitespacePos = line->find(" ");
		while (whitespacePos != std::string::npos) {
			line->replace(whitespacePos, 1, "");
			whitespacePos = line->find(" ");
		}
		whitespacePos = line->find("\t");
		while (whitespacePos != std::string::npos) {
			line->replace(whitespacePos, 1, "");
			whitespacePos = line->find("\t");
		}
		whitespacePos = line->find("\n");

		std::string durationStr, inputStr;
		size_t splitPos = line->find(":");
		if (splitPos == std::string::npos) {
			sprintf_s(&errorMessage[0], 128, "Line %d: malformed frame.", currentLine);
			return false;
		}
		durationStr = line->substr(0, splitPos);
		inputStr = line->substr(splitPos + 1);

		bool multiFrame = false;

		size_t durationStart = 0, durationEnd = 0;
		size_t durationSplitPos = durationStr.find("-");
		if (durationSplitPos != std::string::npos) {
			if (durationStr.substr(0, durationSplitPos).size() == 0 || durationStr.substr(durationSplitPos + 1).size() == 0) {
				sprintf_s(&errorMessage[0], 128, "Line %d: malformed frame range.", currentLine);
				return false;
			}
			multiFrame = true;
			durationStart = atoi(durationStr.substr(0, durationSplitPos).c_str()) - reversalOffset;
			durationEnd = atoi(durationStr.substr(durationSplitPos + 1).c_str()) - reversalOffset;
		}
		else {
			if (durationStr.size() == 0) {
				sprintf_s(&errorMessage[0], 128, "Line %d: missing frame number.", currentLine);
				return false;
			}
			durationStart = atoi(durationStr.c_str()) - reversalOffset;
		}

		std::string directionChar = inputStr.substr(0, 1);
		unsigned char direction = 0;
		if (isdigit(directionChar.c_str()[0])) {
			direction = atoi(directionChar.c_str());
			if (direction == 5) direction = 0;
		}

		bool buttonA = (inputStr.find("A") != std::string::npos) || (inputStr.find("a") != std::string::npos);
		bool buttonB = (inputStr.find("B") != std::string::npos) || (inputStr.find("b") != std::string::npos);
		bool buttonC = (inputStr.find("C") != std::string::npos) || (inputStr.find("c") != std::string::npos);
		bool buttonD = (inputStr.find("D") != std::string::npos) || (inputStr.find("d") != std::string::npos);

		unsigned char buttons = (buttonA << 0) | (buttonB << 1) | (buttonC << 2) | (buttonD << 3);

		if (multiFrame) {
			size_t length = durationEnd - durationStart;
			for (unsigned int i = 0; i <= length; i++) {
				dummyData[((durationStart + i) * 4)] = buttons;
				dummyData[((durationStart + i) * 4) + 1] = direction;
			}
			numFrames = durationEnd;
		}
		else {
			dummyData[(durationStart * 4)] = buttons;
			dummyData[(durationStart * 4) + 1] = direction;
			numFrames = durationStart;
		}
	}

	*sizeOut = (numFrames + 1) * 4;
	return true;
}

std::string Parser::toFile(unsigned char* dummyData, size_t size) {
	std::stringstream buffer;
	unsigned int lastDirection = -1;
	unsigned int lastButtons = -1;

	unsigned int currentFrame = 0;
	unsigned int startBlockFrame = -1;
	
	bool multiFrame = false;
	while (currentFrame < (size / sizeof(int))) {
		unsigned int currentInput = (dummyData[(currentFrame * 4)]) | (dummyData[(currentFrame * 4) + 1] << 8);

		if (currentInput == 0 && currentFrame != ((size / sizeof(int)) - 1)) {
			currentFrame++;
			continue;
		}

		// happens only at the last frame to pad out and keep dummy data size consistent
		// last line of the file
		if (currentInput == 0 && currentFrame == (size / sizeof(int) - 1)) {
			buffer << currentFrame << ": 5\n";
			break;
		}

		unsigned int nextInput = (dummyData[((currentFrame + 1) * 4)]) | (dummyData[((currentFrame + 1) * 4) + 1] << 8);
		
		if (nextInput == currentInput) {
			if (startBlockFrame == -1) {
				startBlockFrame = currentFrame;
			}
			multiFrame = true;
			currentFrame++;
			continue;
		}

		char directionChar[2] = "5";
		unsigned char direction = (unsigned char)((currentInput & 0xFF00) >> 8);
		if (direction != 0) {
			_itoa_s(direction, directionChar, 2, 10);
		}
		
		unsigned char buttons = (unsigned char)(currentInput & 0xFF);

		if (buttons || direction) {
			if (multiFrame) {
				buffer << startBlockFrame << "-" << currentFrame << ": "
					<< directionChar
					<< (buttons & 0x1 ? "A" : "")
					<< (buttons & 0x2 ? "B" : "")
					<< (buttons & 0x4 ? "C" : "")
					<< (buttons & 0x8 ? "D" : "")
					<< "\n";
				startBlockFrame = -1;
				multiFrame = false;
			}
			else {
				buffer << currentFrame << ": "
					<< directionChar
					<< (buttons & 0x1 ? "A" : "")
					<< (buttons & 0x2 ? "B" : "")
					<< (buttons & 0x4 ? "C" : "")
					<< (buttons & 0x8 ? "D" : "")
					<< "\n";
			}
			currentFrame++;
			continue;
		}
	}
	return buffer.str();
}