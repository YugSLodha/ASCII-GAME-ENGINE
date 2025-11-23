#include "engine.h"

int COLS = 48;
int ROWS = 27;
const double fps = 60;
double dt;

std::vector<std::wstring> grid;

void DrawBorder() {
	std::fill(grid[0].begin(), grid[0].end(), L'#');
	std::fill(grid[ROWS - 1].begin(), grid[ROWS - 1].end(), L'#');
	for (int y = 1; y < ROWS - 1; y++)
	{
		grid[y][0] = L'#';
		grid[y][COLS - 1] = L'#';
	}
}

void ClearGrid() { grid.resize(ROWS, std::wstring(COLS, L' ')); }

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
	HWND hwnd = CreateDisplayWindow(hInstance);

	MSG msg = {};
	bool running = true;

	while (running) {
		ClearGrid();
		DrawBorder();
		InvalidateRect(hwnd, NULL, false);
		UpdateWindow(hwnd);

		dt = Tick(fps);

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				running = false;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}