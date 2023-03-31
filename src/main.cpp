#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <resources/resource.h>
#include "ui.hpp"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {
	DialogBox(hInstance, MAKEINTRESOURCE(MainWindow), 0, UI::windowProc);
	return 0;
}