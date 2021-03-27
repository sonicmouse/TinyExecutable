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

// since we got rid of intrinsic functions, we have to implement our own very inefficient versions.
extern "C" void* __cdecl memset(void* _dst, int _val, size_t _size);
#pragma intrinsic(memset)
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/// <summary>
/// We cut out the CRT, so there is no initialization stub. As a matter of fact, this is the stub.
/// We are just going to run from here.
/// </summary>
int WINAPI NakedMain() {

	const HINSTANCE hInstance = GetModuleHandle(NULL);

	WNDCLASSEX wcex{ sizeof(WNDCLASSEX) };
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_3DSHADOW + 1);
	wcex.lpszClassName = "SMOL";

	RegisterClassEx(&wcex);

	HWND hWnd = CreateWindow(wcex.lpszClassName, wcex.lpszClassName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd) {
		return -1;
	}

	// this gets the nCmdShow for ShowWindow()
	STARTUPINFO sui{ sizeof(STARTUPINFO) };
	GetStartupInfo(&sui);

	ShowWindow(hWnd, (sui.dwFlags & STARTF_USESHOWWINDOW) ? sui.wShowWindow : SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	// skipping translator table. No resources == smaller exe.

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message) {
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_PAINT:
			{
				PAINTSTRUCT ps{};
				HDC hdc = BeginPaint(hWnd, &ps);

				SetBkMode(hdc, TRANSPARENT);

				RECT rc{};
				GetClientRect(hWnd, &rc);

				DrawText(
					hdc,
					"1,824 BYTES",
					-1,
					&rc,
					DT_SINGLELINE | DT_CENTER | DT_VCENTER);

				EndPaint(hWnd, &ps);
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

#pragma function(memset)
void* __cdecl memset(void* _dst, int _val, size_t _size) {
	_asm {
		push ecx
		push edi

		mov eax, _val
		mov ecx, _size
		mov edi, _dst
		rep stosb

		pop edi
		pop ecx
	}
	return _dst;
}
