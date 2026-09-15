#include "graphics.h"

// main brains
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_DESTROY) {
		uGraphics::Exit();
		PostQuitMessage(0); return 0;
	}
	switch (uMsg)
	{
	case WM_CREATE:
		uGraphics::Init(hwnd);
		uGraphics::CreateBrush();
		break;
	case WM_PAINT:
		uGraphics::DrawFractal();
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevInstance,
	_In_ LPWSTR cmd, _In_ int nCmdShow) {

	WNDCLASSEX windowClass;
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowClass.hInstance = hInstance;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.lpszClassName = "MainWindow";
	windowClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&windowClass);

	// create handle
	HWND hwindowHandle = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		windowClass.lpszClassName,
		"DirectX 2D", WS_OVERLAPPED,
		100, 200, 300, 250,
		NULL, NULL, hInstance, NULL);

	if (hwindowHandle == NULL) { return 0; }
	ShowWindow(hwindowHandle, nCmdShow);

	// start response loop to windows
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}