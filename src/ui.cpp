#include "ui.hpp"

#include <Windows.h>
#include <string>
#include <sstream>
#include <commdlg.h>
#include <ShObjIdl.h>

#include <resources/resource.h>
#include "definitions.hpp"

// resource ID for the status bar is kept outside resource.h because otherwise any updates to it delete it or even end up generating conflicting resource IDs with it
#define StatusText						1200

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "game.hpp"
#include "parser.hpp"

namespace UI {
	static HWND window;
	static HWND statusText;
	static HWND Slot1_Save_Button;
	static HWND Slot2_Save_Button;
	static HWND Slot3_Save_Button;
	static HWND Slot4_Save_Button;
	static HWND Slot5_Save_Button;

	static HWND Slot1_Load_Button;
	static HWND Slot2_Load_Button;
	static HWND Slot3_Load_Button;
	static HWND Slot4_Load_Button;
	static HWND Slot5_Load_Button;

	static HWND Slot1_LoadReversal_Button;
	static HWND Slot2_LoadReversal_Button;
	static HWND Slot3_LoadReversal_Button;
	static HWND Slot4_LoadReversal_Button;
	static HWND Slot5_LoadReversal_Button;

	static HWND Slot1_Clear_Button;
	static HWND Slot2_Clear_Button;
	static HWND Slot3_Clear_Button;
	static HWND Slot4_Clear_Button;
	static HWND Slot5_Clear_Button;

	static HWND Slot1_FrameCount_Text;
	static HWND Slot2_FrameCount_Text;
	static HWND Slot3_FrameCount_Text;
	static HWND Slot4_FrameCount_Text;
	static HWND Slot5_FrameCount_Text;
}

void UI::setStatusText(std::string text) {
	SetWindowTextA(statusText, text.c_str());
}

void UI::disableAllButtons() {
	EnableWindow(Slot1_Save_Button, false);
	EnableWindow(Slot2_Save_Button, false);
	EnableWindow(Slot3_Save_Button, false);
	EnableWindow(Slot4_Save_Button, false);
	EnableWindow(Slot5_Save_Button, false);

	EnableWindow(Slot1_Load_Button, false);
	EnableWindow(Slot2_Load_Button, false);
	EnableWindow(Slot3_Load_Button, false);
	EnableWindow(Slot4_Load_Button, false);
	EnableWindow(Slot5_Load_Button, false);

	EnableWindow(Slot1_LoadReversal_Button, false);
	EnableWindow(Slot2_LoadReversal_Button, false);
	EnableWindow(Slot3_LoadReversal_Button, false);
	EnableWindow(Slot4_LoadReversal_Button, false);
	EnableWindow(Slot5_LoadReversal_Button, false);

	EnableWindow(Slot1_Clear_Button, false);
	EnableWindow(Slot2_Clear_Button, false);
	EnableWindow(Slot3_Clear_Button, false);
	EnableWindow(Slot4_Clear_Button, false);
	EnableWindow(Slot5_Clear_Button, false);

	SetWindowTextA(Slot1_FrameCount_Text, "");
	SetWindowTextA(Slot2_FrameCount_Text, "");
	SetWindowTextA(Slot3_FrameCount_Text, "");
	SetWindowTextA(Slot4_FrameCount_Text, "");
	SetWindowTextA(Slot5_FrameCount_Text, "");
}

void UI::enableLoadButtons() {
	EnableWindow(Slot1_Load_Button, true);
	EnableWindow(Slot2_Load_Button, true);
	EnableWindow(Slot3_Load_Button, true);
	EnableWindow(Slot4_Load_Button, true);
	EnableWindow(Slot5_Load_Button, true);

	EnableWindow(Slot1_LoadReversal_Button, true);
	EnableWindow(Slot2_LoadReversal_Button, true);
	EnableWindow(Slot3_LoadReversal_Button, true);
	EnableWindow(Slot4_LoadReversal_Button, true);
	EnableWindow(Slot5_LoadReversal_Button, true);
}

bool UI::openFileIntoSlot(HWND hWnd, unsigned int slot, bool reversalTiming) {
	std::string fileName = "";
	fileName.resize(MAX_PATH);
	OPENFILENAMEA ofn = {};
	ofn.lStructSize = sizeof(OPENFILENAMEA);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = &(fileName[0]);
	ofn.nMaxFile = fileName.size();
	ofn.lpstrFilter = "Text files (*.txt)\0*.txt\0All files (*.*)\0*.*\0\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (GetOpenFileNameA(&ofn) != TRUE) {
		return false;
	}

	std::string infoText(128, '\0');
	sprintf_s(&infoText[0], 128, "Loading into slot %d.", slot);
	setStatusText(infoText);

	if (Game::importDummyData(fileName.c_str(), slot, reversalTiming)) {
		sprintf_s(&infoText[0], 128, "Loaded into slot %d.", slot);
	}
	else {
		sprintf_s(&infoText[0], 128, "Error: %s", Parser::getErrorMessage().c_str());
	}

	setStatusText(infoText);

	buildDummyDataDisplay();

	return true;
}

bool UI::saveFileFromSlot(HWND hWnd, unsigned int slot) {
	std::string fileName = "";
	fileName.resize(MAX_PATH);
	OPENFILENAMEA ofn = {};
	ofn.lStructSize = sizeof(OPENFILENAMEA);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = &(fileName[0]);
	ofn.nMaxFile = fileName.size();
	ofn.lpstrFilter = "Text files (*.txt)\0*.txt\0All files (*.*)\0*.*\0\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.lpstrInitialDir = NULL;
	if (GetSaveFileNameA(&ofn) != TRUE) {
		return false;
	}

	std::string infoText(256, '\0');
	sprintf_s(&infoText[0], 256, "Exporting slot %d.", slot);
	setStatusText(infoText.c_str());
	if (Game::exportDummyData(slot, fileName.c_str())) {
		sprintf_s(&infoText[0], 256, "Exported slot %d.", slot);
	}
	else {
		sprintf_s(&infoText[0], 256, "Failed to export slot %d.", slot);
	}
	setStatusText(infoText.c_str());

	return true;
}

bool UI::clearSlot(unsigned int slot) {
	Game::clearDummyData(slot);
	std::string infoText(256, '\0');
	sprintf_s(&infoText[0], 256, "Cleared slot %d.", slot);
	setStatusText(infoText.c_str());
	buildDummyDataDisplay();

	return true;
}

void UI::toggleSaveButton(unsigned int slot, bool enable) {
	switch (slot) {
	case 1: EnableWindow(Slot1_Save_Button, enable); break;
	case 2: EnableWindow(Slot2_Save_Button, enable); break;
	case 3: EnableWindow(Slot3_Save_Button, enable); break;
	case 4: EnableWindow(Slot4_Save_Button, enable); break;
	case 5: EnableWindow(Slot5_Save_Button, enable); break;
	default:
		break;
	}
}

void UI::buildDummyDataDisplay() {
	if (!Game::isActive() && !Game::initialize()) {
		setStatusText("Waiting for Lumina to open...");
		disableAllButtons();
		return;
	}

	enableLoadButtons();
	char frameCountString[10] = "";
	unsigned int frameCount = 0;

	frameCount = Game::getDummyDataSize(1) / 4;
	if (frameCount > 0) {
		sprintf_s(frameCountString, 10, "%d f", frameCount);
		EnableWindow(Slot1_Save_Button, true);
		EnableWindow(Slot1_Clear_Button, true);
	}
	else {
		sprintf_s(frameCountString, 10, "<empty>");
		EnableWindow(Slot1_Save_Button, false);
		EnableWindow(Slot1_Clear_Button, false);
	}
	SetWindowTextA(Slot1_FrameCount_Text, frameCountString);

	frameCount = Game::getDummyDataSize(2) / 4;
	if (frameCount > 0) {
		sprintf_s(frameCountString, 10, "%d f", frameCount);
		EnableWindow(Slot2_Save_Button, true);
		EnableWindow(Slot2_Clear_Button, true);
	}
	else {
		sprintf_s(frameCountString, 10, "<empty>");
		EnableWindow(Slot2_Save_Button, false);
		EnableWindow(Slot2_Clear_Button, false);
	}
	SetWindowTextA(Slot2_FrameCount_Text, frameCountString);

	frameCount = Game::getDummyDataSize(3) / 4;
	if (frameCount > 0) {
		sprintf_s(frameCountString, 10, "%d f", frameCount);
		EnableWindow(Slot3_Save_Button, true);
		EnableWindow(Slot3_Clear_Button, true);
	}
	else {
		sprintf_s(frameCountString, 10, "<empty>");
		EnableWindow(Slot3_Save_Button, false);
		EnableWindow(Slot3_Clear_Button, false);
	}
	SetWindowTextA(Slot3_FrameCount_Text, frameCountString);

	frameCount = Game::getDummyDataSize(4) / 4;
	if (frameCount > 0) {
		sprintf_s(frameCountString, 10, "%d f", frameCount);
		EnableWindow(Slot4_Save_Button, true);
		EnableWindow(Slot4_Clear_Button, true);
	}
	else {
		sprintf_s(frameCountString, 10, "<empty>");
		EnableWindow(Slot4_Save_Button, false);
		EnableWindow(Slot4_Clear_Button, false);
	}
	SetWindowTextA(Slot4_FrameCount_Text, frameCountString);

	frameCount = Game::getDummyDataSize(5) / 4;
	if (frameCount > 0) {
		sprintf_s(frameCountString, 10, "%d f", frameCount);
		EnableWindow(Slot5_Save_Button, true);
		EnableWindow(Slot5_Clear_Button, true);
	}
	else {
		sprintf_s(frameCountString, 10, "<empty>");
		EnableWindow(Slot5_Save_Button, false);
		EnableWindow(Slot5_Clear_Button, false);
	}
	SetWindowTextA(Slot5_FrameCount_Text, frameCountString);
}

INT_PTR CALLBACK UI::windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:
	case WM_INITDIALOG: {
		CreateWindowEx(0, STATUSCLASSNAMEW, 0, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, (HMENU)StatusText, GetModuleHandle(0), 0);

		std::stringstream titleStringStream;
		titleStringStream << "Lumina Dummy Data Tool v" << VersionMajor << "." << VersionMinor << "." << VersionPatch;
		std::string titleString = titleStringStream.str();

		UI::window = hWnd;
		SetWindowTextA(hWnd, titleString.c_str());
		UI::statusText = GetDlgItem(hWnd, StatusText);

		UI::Slot1_FrameCount_Text = GetDlgItem(hWnd, Slot1_FrameCount);
		UI::Slot2_FrameCount_Text = GetDlgItem(hWnd, Slot2_FrameCount);
		UI::Slot3_FrameCount_Text = GetDlgItem(hWnd, Slot3_FrameCount);
		UI::Slot4_FrameCount_Text = GetDlgItem(hWnd, Slot4_FrameCount);
		UI::Slot5_FrameCount_Text = GetDlgItem(hWnd, Slot5_FrameCount);

		UI::Slot1_Load_Button = GetDlgItem(hWnd, Slot1_LoadButton);
		UI::Slot2_Load_Button = GetDlgItem(hWnd, Slot2_LoadButton);
		UI::Slot3_Load_Button = GetDlgItem(hWnd, Slot3_LoadButton);
		UI::Slot4_Load_Button = GetDlgItem(hWnd, Slot4_LoadButton);
		UI::Slot5_Load_Button = GetDlgItem(hWnd, Slot5_LoadButton);

		UI::Slot1_LoadReversal_Button = GetDlgItem(hWnd, Slot1_LoadReversalButton);
		UI::Slot2_LoadReversal_Button = GetDlgItem(hWnd, Slot2_LoadReversalButton);
		UI::Slot3_LoadReversal_Button = GetDlgItem(hWnd, Slot3_LoadReversalButton);
		UI::Slot4_LoadReversal_Button = GetDlgItem(hWnd, Slot4_LoadReversalButton);
		UI::Slot5_LoadReversal_Button = GetDlgItem(hWnd, Slot5_LoadReversalButton);

		UI::Slot1_Save_Button = GetDlgItem(hWnd, Slot1_SaveButton);
		UI::Slot2_Save_Button = GetDlgItem(hWnd, Slot2_SaveButton);
		UI::Slot3_Save_Button = GetDlgItem(hWnd, Slot3_SaveButton);
		UI::Slot4_Save_Button = GetDlgItem(hWnd, Slot4_SaveButton);
		UI::Slot5_Save_Button = GetDlgItem(hWnd, Slot5_SaveButton);

		UI::Slot1_Clear_Button = GetDlgItem(hWnd, Slot1_ClearButton);
		UI::Slot2_Clear_Button = GetDlgItem(hWnd, Slot2_ClearButton);
		UI::Slot3_Clear_Button = GetDlgItem(hWnd, Slot3_ClearButton);
		UI::Slot4_Clear_Button = GetDlgItem(hWnd, Slot4_ClearButton);
		UI::Slot5_Clear_Button = GetDlgItem(hWnd, Slot5_ClearButton);

		UI::buildDummyDataDisplay();

		break;
	}
	case WM_COMMAND: {
		WORD cmd = LOWORD(wParam);
		switch (cmd) {
		case Slot1_LoadButton: UI::openFileIntoSlot(hWnd, 1); break;
		case Slot2_LoadButton: UI::openFileIntoSlot(hWnd, 2); break;
		case Slot3_LoadButton: UI::openFileIntoSlot(hWnd, 3); break;
		case Slot4_LoadButton: UI::openFileIntoSlot(hWnd, 4); break;
		case Slot5_LoadButton: UI::openFileIntoSlot(hWnd, 5); break;

		case Slot1_LoadReversalButton: UI::openFileIntoSlot(hWnd, 1, true); break;
		case Slot2_LoadReversalButton: UI::openFileIntoSlot(hWnd, 2, true); break;
		case Slot3_LoadReversalButton: UI::openFileIntoSlot(hWnd, 3, true); break;
		case Slot4_LoadReversalButton: UI::openFileIntoSlot(hWnd, 4, true); break;
		case Slot5_LoadReversalButton: UI::openFileIntoSlot(hWnd, 5, true); break;

		case Slot1_SaveButton: UI::saveFileFromSlot(hWnd, 1); break;
		case Slot2_SaveButton: UI::saveFileFromSlot(hWnd, 2); break;
		case Slot3_SaveButton: UI::saveFileFromSlot(hWnd, 3); break;
		case Slot4_SaveButton: UI::saveFileFromSlot(hWnd, 4); break;
		case Slot5_SaveButton: UI::saveFileFromSlot(hWnd, 5); break;

		case Slot1_ClearButton: UI::clearSlot(1); break;
		case Slot2_ClearButton: UI::clearSlot(2); break;
		case Slot3_ClearButton: UI::clearSlot(3); break;
		case Slot4_ClearButton: UI::clearSlot(4); break;
		case Slot5_ClearButton: UI::clearSlot(5); break;

		default:
			break;
		}
		break;
	}
	case WM_ACTIVATE:
	case WM_SETFOCUS: {
		UI::buildDummyDataDisplay();
		break;
	}
	case WM_CLOSE:
		DestroyWindow(hWnd);
		Game::exit();
		exit(0);
		break;
	default:
		break;
	}
	return (INT_PTR)FALSE;
}