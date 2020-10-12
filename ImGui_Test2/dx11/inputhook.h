#pragma once

#include "../framework.h"

namespace DX11
{
	class InputHook
	{
	public:
		static WNDPROC oWndProc;
		static bool bBlockInput;

		static void LoadHook(HWND);
		static void UnloadHook(HWND);
		static LRESULT APIENTRY WndProc(HWND, UINT, WPARAM, LPARAM);
	};
}