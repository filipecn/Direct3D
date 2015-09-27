#pragma once

#include "common/D3DHeaders.h"

#include <string>
#include <memory>
#include <functional>

#include "common/NumericalTypes.hpp"

namespace aergiaX {
	class GraphicsDisplay {
	public:
		static GraphicsDisplay& instance();

		virtual ~GraphicsDisplay();

		static GraphicsDisplay& create(uint width, uint height, std::string name, HINSTANCE hInstance, int cmdShow);

		// window size in pixels
		uint w_width, w_height;
		std::string w_title;
		HWND hwnd;
		D3D_DRIVER_TYPE m_driverType;
		D3D_FEATURE_LEVEL m_featureLevel;
		// direct3D
		Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dContext;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_backBufferTarget;

		void setupDirect3D();
		int run();

		// callbacks
		std::function<void()> renderCallback;

	private:
		GraphicsDisplay();

		// window callback procedure
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};
}