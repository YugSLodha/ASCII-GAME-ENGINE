#pragma once
#ifndef ENGINE_H
#define ENGINE_H



// =========================
// DEPENDENCIES
// =========================
#include <windows.h>
#include <vector>
#include <iostream>



// =========================
// VARIABLES
// =========================
extern int ROWS;
extern int COLS;
extern std::vector<std::wstring> grid;
int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);
int CELL_SIZE;
void InnitCell() { CELL_SIZE = std::min(screenWidth / COLS, screenHeight / ROWS); }

static HFONT gFont = NULL;
static int gFontAdvance = 0;

static HDC gMemDC = NULL;
static HBITMAP gMemBitmap = NULL;
static HBITMAP gOldBitmap = NULL;

static LARGE_INTEGER g_Freq;
static LARGE_INTEGER g_Last;
static bool g_TimeInitialized = false;



// =========================
// UTILITY FUNCTIONS
// =========================
void DrawBorder() {
    std::fill(grid[0].begin(), grid[0].end(), L'#');
    std::fill(grid[ROWS - 1].begin(), grid[ROWS - 1].end(), L'#');
    for (int y = 1; y < ROWS - 1; y++)
    {
        grid[y][0] = L'#';
        grid[y][COLS - 1] = L'#';
    }
}

void ClearGrid() {
    for (int y = 0; y < ROWS; y++)
        std::fill(grid[y].begin(), grid[y].end(), L' ');
}

void InnitGrid() { grid.resize(ROWS, std::wstring(COLS, L' ')); }



// =========================
// WINDOW DRAWING
// =========================
void CreateScaledFont(int cellSize) {
    int fontHeight = (int)(cellSize * 0.9f);

    gFont = CreateFontW(-fontHeight, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH, L"Terminal");
}

LRESULT CALLBACK DisplayWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        CreateScaledFont(CELL_SIZE);
        HDC hdc = GetDC(hwnd);
        gMemDC = CreateCompatibleDC(hdc);
        gMemBitmap = CreateCompatibleBitmap(hdc, COLS * CELL_SIZE, ROWS * CELL_SIZE);
        gOldBitmap = (HBITMAP)SelectObject(gMemDC, gMemBitmap);
        SelectObject(gMemDC, gFont);
        ABC abc;
        GetCharABCWidthsW(gMemDC, L'W', L'W', &abc);
        gFontAdvance = abc.abcA + abc.abcB + abc.abcC;
        ReleaseDC(hwnd, hdc);
        return 0;
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        HBRUSH hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
        RECT rect = { 0, 0, COLS * CELL_SIZE, ROWS * CELL_SIZE };
        FillRect(gMemDC, &rect, hBrush);
        SelectObject(gMemDC, gFont);
        SetTextColor(gMemDC, RGB(255, 255, 255));
        SetBkColor(gMemDC, RGB(0, 0, 0));
        for (int y = 0; y < ROWS; y++) {
            for (int x = 0; x < COLS; x++) {
                wchar_t ch[2] = { grid[y][x], 0 };
                TextOutW(gMemDC, x * CELL_SIZE, y * CELL_SIZE, ch, 1);
            }
        }
        BitBlt(hdc, 0, 0, COLS * CELL_SIZE, ROWS * CELL_SIZE, gMemDC, 0, 0, SRCCOPY);

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_DESTROY: {
        if (gMemDC) {
            SelectObject(gMemDC, gOldBitmap);
            DeleteObject(gMemBitmap);
            DeleteDC(gMemDC);
        }
        DeleteObject(gFont);
        PostQuitMessage(0);
        return 0;
    }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

HWND CreateDisplayWindow(HINSTANCE hInstance) {
    const wchar_t CLASS_NAME[] = L"DisplayWindowClass";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = DisplayWindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    RegisterClassW(&wc);

    return CreateWindowExW(0, CLASS_NAME, L"Display Grid (Full Screen)", WS_POPUP | WS_VISIBLE, 0, 0, screenWidth, screenHeight, NULL, NULL, hInstance, NULL);
}

bool MessageHandler(MSG& msg) {
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT)
            return false;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}



// =========================
// FPS CONTROL
// =========================
static double Tick(double targetFPS) {
    if (!g_TimeInitialized) {
        QueryPerformanceFrequency(&g_Freq);
        QueryPerformanceCounter(&g_Last);
        g_TimeInitialized = true;
        return 0.0;
    }

    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);

    double dt = (double)(now.QuadPart - g_Last.QuadPart) / g_Freq.QuadPart;

    double targetFrame = 1.0 / targetFPS;

    while (dt < targetFrame) {
        Sleep(0);
        QueryPerformanceCounter(&now);
        dt = (double)(now.QuadPart - g_Last.QuadPart) / g_Freq.QuadPart;
    }

    g_Last = now;
    return dt;
}

#endif