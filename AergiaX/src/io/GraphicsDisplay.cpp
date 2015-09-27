#include "GraphicsDisplay.h"
#include "utils/DebugMessage.hpp"
#include "utils/CheckDxError.hpp"

#include <cassert>
#include <chrono>
using std::chrono::milliseconds;
using std::chrono::duration;
using std::chrono::steady_clock;
using std::chrono::duration_cast;
#include <thread>

namespace aergiaX {
	GraphicsDisplay::GraphicsDisplay() {
		renderCallback = nullptr;
	}

	GraphicsDisplay::~GraphicsDisplay() {}

	GraphicsDisplay& GraphicsDisplay::instance() {
		static GraphicsDisplay gd;
		return gd;
	}

	GraphicsDisplay& GraphicsDisplay::create(uint width, uint height, std::string name,
		HINSTANCE hInstance, int cmdShow) {
		GraphicsDisplay& gd = instance();
		gd.w_width = width;
		gd.w_height = height;
		gd.w_title = name;
		gd.m_driverType = D3D_DRIVER_TYPE_NULL;
		gd.m_featureLevel = D3D_FEATURE_LEVEL_11_0;

		// window registration and creation
		WNDCLASSEX wndClass = { 0 };
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = WndProc;
		wndClass.hInstance = hInstance;
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = L"DX11WindowClass";

		// Register a window class with name "DX11WindowClass"
		if (!RegisterClassEx(&wndClass))
			DEBUG_MSG(L"Failed on window registration.");

		RECT windowRect = {
			0L,
			0L,
			static_cast<long>(gd.w_width),
			static_cast<long>(gd.w_height)
		};
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

		// Now create the window from the previously registered "DX11WindowClass" class 
		gd.hwnd = CreateWindowA(
			"DX11WindowClass",
			gd.w_title.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top,
			NULL,
			NULL,
			hInstance,
			NULL
			);

		if (!gd.hwnd)
			DEBUG_MSG(L"Failed on window creation.");

		ShowWindow(gd.hwnd, cmdShow);

		gd.setupDirect3D();

		return instance();
	}

	LRESULT CALLBACK GraphicsDisplay::WndProc(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam) {
		PAINTSTRUCT paintStruct;
		HDC hDC;

		switch (message) {
		case WM_PAINT:
			hDC = BeginPaint(hWindow, &paintStruct);
			EndPaint(hWindow, &paintStruct);
			break;

		case WM_DESTROY:
			// Send a WM_QUIT message to terminate our application loop within wWinMain().
			PostQuitMessage(0);
			break;

		case WM_KEYUP:
		case WM_KEYDOWN:
			//keyInputCallBack(hWindow, message, wParam);
			break;
		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_XBUTTONDOWN:
			//mouseButtonCallBack(hWindow, message, wParam);
			break;
		default:
			// Pass unprocessed messages to the default windows procedure callback.
			return DefWindowProc(hWindow, message, wParam, lParam);
		}
		return 0;
	}

	void GraphicsDisplay::setupDirect3D() {
		// we need to complete de following
		// 1. Define the device types and feature levels we want to check for.
		// 2. Create the Direct3D device, rendering context, and swap chain.
		// 3. Create the render target.
		// 4. Set the viewport.

		// Item 1 - Define the device types and feature levels we want to check for.
		RECT dimensions;
		GetClientRect(hwnd, &dimensions);
		uint width = dimensions.right - dimensions.left;
		uint height = dimensions.bottom - dimensions.top;

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
			D3D_DRIVER_TYPE_SOFTWARE
		};
		uint totalDriverTypes = ARRAYSIZE(driverTypes);
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};
		uint totalFeatureLevels = ARRAYSIZE(featureLevels);

		// Item 2 - Create the Direct3D device, rendering context, and swap chain.
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = height;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hwnd;
		swapChainDesc.Windowed = true;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		uint creationFlags = 0;
#ifdef _DEBUG
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		HRESULT result;
		for (uint driverIndex(0); driverIndex < totalDriverTypes; ++driverIndex) {
			result = D3D11CreateDeviceAndSwapChain(
				0,
				driverTypes[driverIndex],
				0,
				creationFlags,
				featureLevels,
				totalFeatureLevels,
				D3D11_SDK_VERSION,
				&swapChainDesc,
				&m_swapChain,
				&m_d3dDevice,
				&m_featureLevel,
				&m_d3dContext
				);

			if (SUCCEEDED(result)) {
				m_driverType = driverTypes[driverIndex];
				break;
			}
		}
		if (FAILED(result)) {
			DEBUG_MSG(L"Failed to create the Direct3D device.");
		}

		// 3. Create the render target.
		ID3D11Texture2D * backBufferTexture = nullptr;

		CHECK_DX_ERROR(
			m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBufferTexture))
			);

		CHECK_DX_ERROR(
			m_d3dDevice->CreateRenderTargetView(backBufferTexture, nullptr, &m_backBufferTarget);
		);

		if (backBufferTexture) backBufferTexture->Release();

		m_d3dContext->OMSetRenderTargets(1, m_backBufferTarget.GetAddressOf(), nullptr);
		assert(m_backBufferTarget);

		// 4. Set the viewport.
		D3D11_VIEWPORT viewport;
		viewport.Width = static_cast<float>(width);
		viewport.Height = static_cast<float>(height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;


		m_d3dContext->RSSetViewports(1, &viewport);
	}

	void frameLimiter(float desiredFramesPerSecond, const steady_clock::time_point & frameStartTime) {
		// Convert to milliseconds per frame
		uint desiredMsPerFrame = uint(1000.0f / desiredFramesPerSecond);

		steady_clock::time_point t2(steady_clock::now());
		milliseconds elapsedTime(duration_cast<milliseconds>(t2 - frameStartTime));

		milliseconds d(desiredMsPerFrame);
		if (elapsedTime < milliseconds(desiredMsPerFrame)) {
			// TODO Dustin - Build error on this line:
			std::this_thread::sleep_for(d - elapsedTime);
		}
	}

	int GraphicsDisplay::run() {
		// Begin the main message processing and rendering loop.
		MSG msg = { 0 };

		steady_clock::time_point frameStartTime;

		while (msg.message != WM_QUIT) {
			frameStartTime = steady_clock::now();

			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
				// Translates input virtual key message to character message.
				TranslateMessage(&msg);

				// Dispatch message to our registered Window Procedure Callback function.
				DispatchMessage(&msg);
			}
			else {
				float dt = 0.0f;

				if (renderCallback)
					renderCallback();
			}

			frameLimiter(60.0, frameStartTime);
		}

		// Release resources
		// shutdown();

		return static_cast<int>(msg.wParam);
	}
}