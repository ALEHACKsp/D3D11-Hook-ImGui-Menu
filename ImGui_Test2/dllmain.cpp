#include "framework.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		AllocConsole();
		FILE* file = nullptr;
		freopen_s(&file, "CONOUT$", "w", stdout);
		ShowWindow(GetConsoleWindow(), SW_SHOW);

		DX11::LoadHook();
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		DX11::UnloadHook();
	}
	return TRUE;
}

