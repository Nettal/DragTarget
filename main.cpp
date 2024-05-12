//
//	MAIN.CPP
//
//	Freeware written by J Brown 2004
//
//	www.catch22.net
//

#define STRICT

#include <windows.h>
#include "resource.h"

#define APPNAME "IDropTarget"

#pragma comment(linker, "/OPT:NOWIN98")

HWND hwndMain;
HWND hwndEdit;
HINSTANCE hInstance;

void RegisterDropWindow(HWND hwnd, IDropTarget **ppDropTarget);

void UnregisterDropWindow(HWND hwnd, IDropTarget *pDropTarget);

BOOL DropClipboardData(HWND hwnd);

char szTextBuffer[200];

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static IDropTarget *pDropTarget;
    switch (msg) {
        case WM_CREATE:

            hwndEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
                                      WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL,
                                      0, 0, 0, 0, hwnd, 0, hInstance, 0);

            SendMessage(hwndEdit, WM_SETFONT, (WPARAM) GetStockObject(ANSI_FIXED_FONT), 0);

            // make the Edit control into a DropTarget
            RegisterDropWindow(hwndEdit, &pDropTarget);

            SetFocus(hwndEdit);

            return TRUE;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDM_FILE_EXIT:
                    CloseWindow(hwnd);
                    return 0;

                case IDM_FILE_ABOUT:
                    MessageBox(hwnd, "IDropTarget Test Application\r\n\r\n"
                                     "Copyright(c) 2004 by Catch22 Productions\t\r\n"
                                     "Written by J Brown.\r\n\r\n"
                                     "Homepage at www.catch22.net", APPNAME, MB_ICONINFORMATION);
                    return 0;
            }

            break;

        case WM_CLOSE:
            // shut program down
            UnregisterDropWindow(hwndEdit, pDropTarget);
            DestroyWindow(hwnd);
            PostQuitMessage(0);
            return 0;

        case WM_SIZE:
            // resize editbox to fit in main window
            MoveWindow(hwndEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
            return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void InitMainWnd() {
    WNDCLASSEX wc = {sizeof(wc)};

    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = APPNAME;
    wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
    wc.hInstance = hInstance;

    RegisterClassEx(&wc);
}

void CreateMainWnd() {
    hwndMain = CreateWindowEx(0, APPNAME, APPNAME,
                              WS_VISIBLE | WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT, CW_USEDEFAULT, 512, 200, 0, 0,
                              hInstance, 0);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nShowCmd) {
    MSG msg;

    hInstance = hInst;

    // This program requires COM
    OleInitialize(0);

    InitMainWnd();
    CreateMainWnd();

    // message-pump
    while (GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Shutdown COM
    OleUninitialize();

    return 0;
}