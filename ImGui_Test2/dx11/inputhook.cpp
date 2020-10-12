#include <inputhook.h>

using namespace ImGui;

#define RET_CHK(x) if (DX11::InputHook::bBlockInput && DX11::Menu::bIsOpen) { return x; }\

namespace DX11
{
	WNDPROC InputHook::oWndProc = 0;
	bool InputHook::bBlockInput = true;

	void InputHook::LoadHook(HWND hWindow) {
		oWndProc = (WNDPROC)SetWindowLongPtr(hWindow, GWLP_WNDPROC, (__int3264)(LONG_PTR)WndProc);
	}

	void InputHook::UnloadHook(HWND hWindow) {
		SetWindowLongPtr(hWindow, GWLP_WNDPROC, (LONG_PTR)oWndProc);
	}

	LRESULT APIENTRY InputHook::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == WM_LBUTTONDOWN) {
			GetIO().MouseDown[0] = true;
			RET_CHK(DefWindowProc(hwnd, uMsg, wParam, lParam));
		}
		else if (uMsg == WM_LBUTTONUP) {
			GetIO().MouseDown[0] = false;
			RET_CHK(DefWindowProc(hwnd, uMsg, wParam, lParam));
		}
		else if (uMsg == WM_RBUTTONDOWN) {
			GetIO().MouseDown[1] = true;
			RET_CHK(DefWindowProc(hwnd, uMsg, wParam, lParam));
		}
		else if (uMsg == WM_RBUTTONUP) {
			GetIO().MouseDown[1] = false;
			RET_CHK(DefWindowProc(hwnd, uMsg, wParam, lParam));
		}
		else if (uMsg == WM_MBUTTONDOWN) {
			GetIO().MouseDown[2] = true;
			RET_CHK(DefWindowProc(hwnd, uMsg, wParam, lParam));
		}
		else if (uMsg == WM_MBUTTONUP) {
			GetIO().MouseDown[2] = false;
			RET_CHK(DefWindowProc(hwnd, uMsg, wParam, lParam));
		}
		else if (uMsg == WM_MOUSEWHEEL) {
			GetIO().MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
			RET_CHK(DefWindowProc(hwnd, uMsg, wParam, lParam));
		}
		else if (uMsg == WM_MOUSEMOVE) {
			GetIO().MousePos.x = (signed short)(lParam);
			GetIO().MousePos.y = (signed short)(lParam >> 16);
			RET_CHK(DefWindowProc(hwnd, uMsg, wParam, lParam));
		}
		return CallWindowProc(oWndProc, hwnd, uMsg, wParam, lParam);
	}
}