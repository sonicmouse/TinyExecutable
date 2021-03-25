//
// Taking on Dave's Garage for smallest functional EXE which displays a window.
// Dave resorted to using UPX to get at 3.5 KB. I can get to 1,776 bytes without any packers... and using C!
// This was written in Visual Studio 2019 Enterprise.
//

#ifndef _DEBUG
#pragma comment(linker, "/NODEFAULTLIB")
#pragma comment(linker, "/INCREMENTAL:NO")
#pragma comment(linker, "/MERGE:.rdata=.text")
#pragma comment(linker, "/ENTRY:NakedMain")
#pragma comment(linker, "/ALIGN:16")
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define CLASS_NAME "TINY_EXE"
#define WINDOW_TITLE "Tiny EXE"
#define DISPLAY_TEXT "THIS IS A REALLY, REALLY SMALL EXECUTABLE. 1,776 BYTES, ACTUALLY!"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/// <summary>
/// We cut out the CRT, so there is no initialization stub. As a matter of fact, this is the stub.
/// We are just going to run from here.
/// </summary>
int WINAPI NakedMain() {

	const HINSTANCE hInstance = GetModuleHandle(NULL);

	WNDCLASSEX wcex{};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_3DSHADOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = CLASS_NAME;
	wcex.hIconSm = NULL;

	RegisterClassEx(&wcex);

	HWND hWnd = CreateWindow(CLASS_NAME, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd){
		return -1;
	}

	// we are not going to worry about nCmdShow and just show the default.
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	// skipping translator table. No resources == smaller exe.

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);

				SetBkMode(hdc, TRANSPARENT);

				RECT rc{};
				GetClientRect(hWnd, &rc);

				DrawText(hdc, DISPLAY_TEXT, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

				EndPaint(hWnd, &ps);
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}