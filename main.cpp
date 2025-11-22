#include "display.h"

int CELL_SIZE = 20;
int ROWS = 15;
int COLS = 30;

std::vector<std::wstring> grid;
void InitGrid()
{
	grid.resize(ROWS);
	for (int y = 0; y < ROWS; y++)
	{
		grid[y].resize(COLS);

		for (int x = 0; x < COLS; x++)
			grid[y][x] = L' ';
	}

	for (int x = 0; x < COLS; x++)
	{
		grid[0][x] = L'#';
		grid[ROWS - 1][x] = L'#';
	}
	for (int y = 0; y < ROWS; y++)
	{
		grid[y][0] = L'#';
		grid[y][COLS - 1] = L'#';
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	HWND hwnd = CreateDisplayWindow(hInstance);

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}