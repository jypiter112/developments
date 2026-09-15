#include <iostream>
#include <MinHook.h>

#pragma comment(lib, "psapi.lib")
#if _WIN64
#ifdef NDEBUG
#pragma comment(lib, "libMinHook-x64-v141-mtd.lib")
#else
#pragma comment(lib, "libMinHook-x64-v141-mt.lib")
#endif
#else
#ifdef NDEBUG
#pragma comment(lib, "libMinHook-x86-v141-mtd.lib")
#else
#pragma comment(lib, "libMinHook-x86-v141-mt.lib")
#endif
#endif

typedef int(__stdcall* MessageBoxW_t)(HWND, LPCWSTR, LPCWSTR, UINT);
MessageBoxW_t originalMessageBoxW = nullptr;
MessageBoxW_t pMessageBoxW;

int __stdcall HookMessageBoxW(HWND hWnd, LPCWSTR lpCaption, LPCWSTR lpText, UINT uType) {
	std::cout << "It Worked\n";
	return originalMessageBoxW(hWnd,lpCaption, lpText, uType);
}

void StartHook() {
	std::cout << "hook starting\n";
	if (MH_Initialize() != MH_OK) {
		std::cout << "Failed to initialize minhook\n";
		return;
	}
	if (MH_CreateHook(pMessageBoxW, &HookMessageBoxW, reinterpret_cast<LPVOID*>(&originalMessageBoxW))!=MH_OK) {
		std::cout << "Hook creation failed\n";
		return;
	}
	if (MH_EnableHook(pMessageBoxW) != MH_OK) {
		std::cout << "Enable fail\n";
		return;
	}
}

int main()
{
	HMODULE hUser32 = LoadLibrary(L"user32.dll");
	if (!hUser32) {
		std::cout << "Couldnt dynamically load user32.dll\n";
		return 1;
	}
	pMessageBoxW = (MessageBoxW_t)GetProcAddress(hUser32, "MessageBoxW");
	if (!pMessageBoxW) {
		FreeLibrary(hUser32);
		return 1;
	}

	StartHook();
	pMessageBoxW(0, L"Hello world!", L"Test", MB_OK);

	MH_DisableHook(&MessageBoxW);
	FreeLibrary(hUser32);
	return 0;
}
