#include "display.h"

int COLS = 96;
int ROWS = 54;

std::vector<std::wstring> grid;

void InitGrid()
{
	grid.resize(ROWS, std::wstring(COLS, L' '));

	std::fill(grid[0].begin(), grid[0].end(), L'#');
	std::fill(grid[ROWS - 1].begin(), grid[ROWS - 1].end(), L'#');
	for (int y = 1; y < ROWS - 1; y++)
	{
		grid[y][0] = L'#';
		grid[y][COLS - 1] = L'#';
	}
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	InitGrid();
	HWND hwnd = CreateDisplayWindow(hInstance);

	MSG msg = {};
	bool running = true;

	while (running) {
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				running = false;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	}


	return 0;
}