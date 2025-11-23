#include "engine.h"

int COLS = 48;
int ROWS = 27;
std::vector<std::wstring> grid;

const double fps = 120;
double dt;

double xpos = 1.0;
double ypos = 1.0;
double speed = 10.0;

Sprite player(std::vector<std::wstring>{ L"X" });

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
	InitCell();
	InitGrid();
	HWND hwnd = CreateDisplayWindow(hInstance);

	MSG msg = {};
	bool running = true;

	while (running) {
		dt = Tick(fps);

		xpos += speed * dt;
		if (xpos >= COLS - 1) { xpos = 1; ypos += 1; }
		if (ypos >= ROWS - 1) { ypos = 1; }

		ClearGrid();
		DrawBorder();
		player.draw((int)xpos, int(ypos));

		InvalidateRect(hwnd, NULL, false);
		UpdateWindow(hwnd);

		running = MessageHandler(msg);
	}

	return 0;
}