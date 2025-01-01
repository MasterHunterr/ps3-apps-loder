#include "pch.h"
#include <windows.h>
#include <urlmon.h>
#include <shlobj.h>
#include <uxtheme.h>
#include <commctrl.h>
#include <mmsystem.h> 
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "uxtheme.lib")
#pragma comment(lib, "winmm.lib") 

constexpr int BUTTON_1 = 101;
constexpr int BUTTON_2 = 102;
constexpr int BUTTON_3 = 103;
constexpr int BUTTON_4 = 104;
constexpr int BUTTON_5 = 105;
constexpr int BUTTON_6 = 106;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
HBRUSH hbrBackground = NULL;

static void PlaySoundEffect(const wchar_t* soundPath) {
    mciSendString(L"close all", NULL, 0, NULL);
    wchar_t command[MAX_PATH];
    swprintf(command, MAX_PATH, L"open \"%s\" type mpegvideo alias mySound", soundPath);
    mciSendString(command, NULL, 0, NULL);
    mciSendString(L"play mySound", NULL, 0, NULL);
}

static void SetNightMode(HWND hwnd) {
    hbrBackground = CreateSolidBrush(RGB(45, 45, 45));
    InvalidateRect(hwnd, NULL, TRUE);
}

static void DownloadFile(const wchar_t* url, const wchar_t* folderPath, const wchar_t* fileName) {
    wchar_t outputFile[MAX_PATH];
    swprintf(outputFile, MAX_PATH, L"%s\\%s", folderPath, fileName);

    HRESULT hr = URLDownloadToFile(NULL, url, outputFile, 0, NULL);

    if (SUCCEEDED(hr)) {
        MessageBox(NULL, L"Download completed successfully!", L"Info", MB_OK | MB_ICONINFORMATION);
        PlaySoundEffect(L"C:\\Windows\\Media\\notify.wav"); 
    }
    else {
        MessageBox(NULL, L"Download failed!", L"Error", MB_OK | MB_ICONERROR);
        PlaySoundEffect(L"C:\\Windows\\Media\\Windows Battery Low.wav"); 
    }
}

static bool GetFolderPath(wchar_t* folderPath, size_t size) {
    BROWSEINFO bi = { 0 };
    bi.lpszTitle = L"Select Download Folder";
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    if (pidl != 0) {
        SHGetPathFromIDList(pidl, folderPath);
        CoTaskMemFree(pidl);
        return true;
    }
    return false;
}

static void AddButtonAnimation(HWND hwndButton) {
    SetWindowLong(hwndButton, GWL_STYLE, GetWindowLong(hwndButton, GWL_STYLE) | BS_FLAT);
    SetWindowTheme(hwndButton, L"", L"");
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"DownloadWindow";
    wc.hbrBackground = hbrBackground = CreateSolidBrush(RGB(45, 45, 45));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (!RegisterClass(&wc))
        return -1;

    HWND hwnd = CreateWindow(
        wc.lpszClassName, L"by MasterHunterr", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 400, NULL, NULL, hInstance, NULL);

    if (!hwnd)
        return -1;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HFONT hFont;
    wchar_t folderPath[MAX_PATH] = L"";

    switch (msg) {
    case WM_CREATE: {
        hFont = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_SWISS, L"Segoe UI");

        HWND hButtons[6];
        const wchar_t* buttonTexts[6] = {
            L"WebMAN", L"multiMAN ", L"Apollo", L"Artemis", L"ManaGuZ", L"GamePad Test"
        };
        int buttonIds[6] = { BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4, BUTTON_5, BUTTON_6 };

        for (int i = 0; i < 6; ++i) {
            hButtons[i] = CreateWindow(L"BUTTON", buttonTexts[i], WS_VISIBLE | WS_CHILD,
                50 + (i % 2) * 200, 50 + (i / 2) * 60, 150, 40,
                hwnd, (HMENU)buttonIds[i], NULL, NULL);
            SendMessage(hButtons[i], WM_SETFONT, (WPARAM)hFont, TRUE);
            AddButtonAnimation(hButtons[i]);
        }

        SetNightMode(hwnd);
        break;
    }

    case WM_CTLCOLORBTN: {
        HDC hdc = (HDC)wParam;
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(255, 255, 255));
        return (INT_PTR)hbrBackground;
    }

    case WM_CTLCOLORDLG:
    case WM_CTLCOLORSTATIC: {
        HDC hdc = (HDC)wParam;
        SetTextColor(hdc, RGB(255, 255, 255));
        SetBkMode(hdc, TRANSPARENT);
        return (INT_PTR)hbrBackground;
    }

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case BUTTON_1:
            if (GetFolderPath(folderPath, MAX_PATH)) {
                DownloadFile(L"https://store.brewology.com/get/homebrew.php?id=310&fid=2426", folderPath, L"WebMAN by DS.pkg");
            }
            break;
        case BUTTON_2:
            if (GetFolderPath(folderPath, MAX_PATH)) {
                DownloadFile(L"https://store.brewology.com/get/homebrew.php?id=24&fid=2412", folderPath, L"multiMAN by DS.pkg");
            }
            break;
        case BUTTON_3:
            if (GetFolderPath(folderPath, MAX_PATH)) {
                DownloadFile(L"https://store.brewology.com/get/homebrew.php?id=321&fid=2428", folderPath, L"Apollo by DS.pkg");
            }
            break;
        case BUTTON_4:
            if (GetFolderPath(folderPath, MAX_PATH)) {
                DownloadFile(L"https://store.brewology.com/get/homebrew.php?id=291&fid=2326t", folderPath, L"Artemis by DS.pkg");
            }
            break;
        case BUTTON_5:
            if (GetFolderPath(folderPath, MAX_PATH)) {
                DownloadFile(L"https://store.brewology.com/get/homebrew.php?id=284&fid=2394", folderPath, L"ManaGuZ by DS.pkg");
            }
            break;
        case BUTTON_6:
            if (GetFolderPath(folderPath, MAX_PATH)) {
                DownloadFile(L"https://store.brewology.com/get/homebrew.php?id=330&fid=2321", folderPath, L"GamePad Test by DS.pkg");
            }
            break;
        }
        break;

    case WM_DESTROY:
        DeleteObject(hbrBackground);
        DeleteObject(hFont);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
