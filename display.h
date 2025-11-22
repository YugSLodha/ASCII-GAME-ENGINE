#ifndef DISPLAY_H
#define DISPLAY_H

#include <windows.h>
#include <vector>
#include <iostream>

extern int CELL_SIZE;
extern int ROWS;
extern int COLS;
extern std::vector<std::wstring> grid;

static HFONT gFont = NULL;

void CreateScaledFont(int cellSize)
{
    int fontHeight = (int)(cellSize * 0.9f);

    gFont = CreateFontW(
        -fontHeight,
        0, 0, 0,
        FW_NORMAL,
        FALSE, FALSE, FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        FIXED_PITCH,
        L"Terminal"
    );
}

LRESULT CALLBACK DisplayWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        CreateScaledFont(CELL_SIZE);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        SelectObject(hdc, gFont);

        SetBkMode(hdc, OPAQUE);
        SetTextColor(hdc, RGB(255, 255, 255));
        SetBkColor(hdc, RGB(0, 0, 0));

        for (int y = 0; y < ROWS; y++)
        {
            for (int x = 0; x < COLS; x++)
            {
                wchar_t ch[2] = { grid[y][x], 0 };

                TextOutW(
                    hdc,
                    x * CELL_SIZE,
                    y * CELL_SIZE,
                    ch,
                    1
                );
            }
        }

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        DeleteObject(gFont);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

HWND CreateDisplayWindow(HINSTANCE hInstance)
{
    const wchar_t CLASS_NAME[] = L"DisplayWindowClass";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = DisplayWindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    RegisterClassW(&wc);

    int width = COLS * CELL_SIZE + 16;
    int height = ROWS * CELL_SIZE + 39;

    return CreateWindowExW(
        0,
        CLASS_NAME,
        L"Display Grid (vector)",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width,
        height,
        NULL, NULL,
        hInstance,
        NULL
    );
}

#endif