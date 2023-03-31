#pragma once

#include <Windows.h>
#include <string>

namespace UI {
	void setStatusText(std::string text);
	void disableAllButtons();
	void enableLoadButtons();
	void toggleSaveButton(unsigned int slot, bool enable);

	bool openFileIntoSlot(HWND hWnd, unsigned int slot, bool reverseTiming = false);
	bool saveFileFromSlot(HWND hWnd, unsigned int slot);
	bool clearSlot(unsigned int slot);
	void buildDummyDataDisplay();
	INT_PTR CALLBACK windowProc(HWND, UINT, WPARAM, LPARAM);
}
