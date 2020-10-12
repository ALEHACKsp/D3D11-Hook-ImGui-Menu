#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <imgui.h>
#include <mutex>
#include <detours.h>
#include <menu.h>
#include <inputhook.h>
#include <dx11.hpp>
#include <d3d11.h>
#include <dxgi.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")