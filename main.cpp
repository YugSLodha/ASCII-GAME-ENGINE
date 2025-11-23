#include "engine.h"

int COLS = 48;
int ROWS = 27;
std::vector<std::wstring> grid;

const double fps = 60;
double dt;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
	InnitCell();
	InnitGrid();
	HWND hwnd = CreateDisplayWindow(hInstance);

	MSG msg = {};
	bool running = true;

	while (running) {
		dt = Tick(fps);

		// GAME LOGIC HERE

		ClearGrid();
		DrawBorder();
		// DRAWING HERE

		InvalidateRect(hwnd, NULL, false);
		UpdateWindow(hwnd);

		running = MessageHandler(msg);
	}

	return 0;
}