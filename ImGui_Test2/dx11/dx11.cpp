#include <dx11.hpp>

const auto MenuKey = VK_DELETE;

namespace DX11
{
#pragma region Members
	typedef HRESULT(__stdcall* D3D11PresentHook) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	typedef HRESULT(__stdcall* D3D11ResizeBuffersHook) (IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
	typedef void(__stdcall* D3D11DrawIndexedHook) (ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
	typedef void(__stdcall* D3D11ClearRenderTargetViewHook) (ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRenderTargetView, const FLOAT ColorRGBA[4]);

	ID3D11Device* pDevice = NULL;
	ID3D11DeviceContext* pContext = NULL;
	IDXGISwapChain* pSwapChain = NULL;
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0, 0, 0, 0 };
	HWND hWnd = 0;
	DWORD dwOld = 0;

	DWORD_PTR* pSwapChainVtable = NULL;
	DWORD_PTR* pDeviceContextVTable = NULL;

	D3D11PresentHook phookD3D11Present = NULL;
	D3D11ResizeBuffersHook phookD3D11ResizeBuffers = NULL;
	D3D11DrawIndexedHook phookD3D11DrawIndexed = NULL;
	D3D11ClearRenderTargetViewHook phookD3D11ClearRenderTargetView = NULL;

	ID3D11Texture2D* pRenderTargetTexture = 0;
	ID3D11RenderTargetView* g_pRenderTargetView = 0;
#pragma endregion

#pragma region Hook
	HRESULT __stdcall hookD3D11Present(IDXGISwapChain* _pSwapChain, UINT SyncInterval, UINT Flags)
	{
		static std::once_flag is_init;

		std::call_once(is_init, [&]() {
			if (SUCCEEDED(_pSwapChain->GetDevice(__uuidof(pDevice), reinterpret_cast<void**>(&pDevice))))
			{
				pDevice->GetImmediateContext(&pContext);
				if (SUCCEEDED(_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pRenderTargetTexture)))
				{
					pDevice->CreateRenderTargetView(pRenderTargetTexture, NULL, &g_pRenderTargetView);
					ImGui::CreateContext();
					ImGuiIO& io = ImGui::GetIO();
					ImGui_ImplWin32_Init(hWnd);
					ImGui_ImplDX11_Init(pDevice, pContext);
					InputHook::LoadHook(hWnd);
					pRenderTargetTexture->Release();
				}
			}
			else
			{
				MessageBoxA(0, "GetDevice failed.", "ERROR", MB_ICONERROR);
			}
			});

		pContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);


		if (GetAsyncKeyState(MenuKey) & 0x1) {
			Menu::bIsOpen = !Menu::bIsOpen;
		}

		InputHook::ValidateHook(hWnd);
		Menu::Render();

		return phookD3D11Present(_pSwapChain, SyncInterval, Flags);
	}

	HRESULT __stdcall hookD3D11ResizeBuffers(IDXGISwapChain* _pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
	{
		ImGui_ImplDX11_InvalidateDeviceObjects();
		if (nullptr != g_pRenderTargetView) 
		{ 
			g_pRenderTargetView->Release(); 
			g_pRenderTargetView = nullptr; 
		}
		HRESULT RetVal = phookD3D11ResizeBuffers(_pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
		if (SUCCEEDED(_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pRenderTargetTexture)))
		{
			pDevice->CreateRenderTargetView(pRenderTargetTexture, NULL, &g_pRenderTargetView);
			ImGui_ImplDX11_CreateDeviceObjects();
			pRenderTargetTexture->Release();
		}
		return RetVal;
	}

	void __stdcall hookD3D11DrawIndexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
	{
		return phookD3D11DrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
	}

	void __stdcall hookD3D11ClearRenderTargetView(ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRenderTargetView, const FLOAT ColorRGBA[4])
	{
		return phookD3D11ClearRenderTargetView(pContext, pRenderTargetView, ColorRGBA);
	}
#pragma endregion

	BOOL WINAPI LoadHook()
	{
		if (!CreateThread(0, 0, [](LPVOID)->DWORD
			{
				while (!hWnd)
				{
					Sleep(120);
					hWnd = FindWindowW(L"UnrealWindow", 0);
					//hWnd = FindWindowW(0, "Specific Window Title");
				}

				ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
				swapChainDesc.BufferCount = 1;
				swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				swapChainDesc.OutputWindow = hWnd;
				swapChainDesc.SampleDesc.Count = 1;
				swapChainDesc.Windowed = TRUE;
				swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
				swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
				swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

				if (FAILED(D3D11CreateDeviceAndSwapChain(
					NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &pSwapChain, &pDevice, NULL, &pContext)))
					return FALSE;

				pSwapChainVtable = reinterpret_cast<DWORD_PTR*>(pSwapChain);
				pSwapChainVtable = reinterpret_cast<DWORD_PTR*>(pSwapChainVtable[0]);
				pDeviceContextVTable = reinterpret_cast<DWORD_PTR*>(pContext);
				pDeviceContextVTable = reinterpret_cast<DWORD_PTR*>(pDeviceContextVTable[0]);
				phookD3D11Present = reinterpret_cast<D3D11PresentHook>(pSwapChainVtable[8]);
				phookD3D11ResizeBuffers = reinterpret_cast<D3D11ResizeBuffersHook>(pSwapChainVtable[13]);
				phookD3D11DrawIndexed = reinterpret_cast<D3D11DrawIndexedHook>(pDeviceContextVTable[12]);
				phookD3D11ClearRenderTargetView = reinterpret_cast<D3D11ClearRenderTargetViewHook>(pDeviceContextVTable[50]);
				if (DetourTransactionBegin() != NO_ERROR ||
					DetourUpdateThread(GetCurrentThread()) != NO_ERROR ||
					DetourAttach(&(PVOID&)phookD3D11Present, hookD3D11Present) != NO_ERROR ||
					DetourAttach(&(PVOID&)phookD3D11ResizeBuffers, hookD3D11ResizeBuffers) != NO_ERROR ||
					DetourAttach(&(PVOID&)phookD3D11DrawIndexed, hookD3D11DrawIndexed) != NO_ERROR ||
					DetourAttach(&(PVOID&)phookD3D11ClearRenderTargetView, hookD3D11ClearRenderTargetView) != NO_ERROR ||
					DetourTransactionCommit() != NO_ERROR)
					return FALSE;

				pDevice->Release();
				pContext->Release();
				pSwapChain->Release();

				return TRUE;
			}, 0, 0, 0))
			return FALSE;
			return TRUE;
	}
	BOOL WINAPI UnloadHook()
	{
		InputHook::UnloadHook(hWnd);
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		if (DetourTransactionBegin() != NO_ERROR ||
			DetourUpdateThread(GetCurrentThread()) != NO_ERROR ||
			DetourDetach(&(PVOID&)phookD3D11Present, hookD3D11Present) != NO_ERROR ||
			DetourDetach(&(PVOID&)phookD3D11ResizeBuffers, hookD3D11ResizeBuffers) != NO_ERROR ||
			DetourDetach(&(PVOID&)phookD3D11DrawIndexed, hookD3D11DrawIndexed) != NO_ERROR ||
			DetourDetach(&(PVOID&)phookD3D11ClearRenderTargetView, hookD3D11ClearRenderTargetView) != NO_ERROR ||
			DetourTransactionCommit() != NO_ERROR)
			return FALSE;
		return TRUE;
	}
}