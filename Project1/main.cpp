#include <iostream>

#include "io/GraphicsDisplay.h"

#include<Windows.h>

void render() {
	aergiaX::io::GraphicsDisplay& gd = aergiaX::io::GraphicsDisplay::instance();
	if (!gd.m_d3dContext) return;
	float clearColor[4] = { 0.0f, 0.0f, 0.25f, 1.0f };

	// Clear the back buffer with the clearColor.
	gd.m_d3dContext->ClearRenderTargetView(gd.m_backBufferTarget.Get(), clearColor);

	// Swap the back and front buffers.
	gd.m_swapChain->Present(0, 0);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	LPWSTR cmdLine, int cmdShow)
{
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);

	aergiaX::io::GraphicsDisplay& gd = aergiaX::io::GraphicsDisplay::create(800, 800, std::string("BlankDemo"), hInstance, cmdShow);
	gd.renderCallback = render;
	return gd.run();
}