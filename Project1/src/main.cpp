#include <iostream>

#include "io/GraphicsDisplay.h"
#include "graphics/scene/SceneObject.h"

#include<Windows.h>

aergiaX::SceneObject so;

void render() {
	aergiaX::GraphicsDisplay& gd = aergiaX::GraphicsDisplay::instance();
	if (!gd.m_d3dContext) return;
	float clearColor[4] = { 0.0f, 0.0f, 0.25f, 1.0f };

	// Clear the back buffer with the clearColor.
	gd.m_d3dContext->ClearRenderTargetView(gd.m_backBufferTarget.Get(), clearColor);

	so.render(gd.m_d3dContext);

	// Swap the back and front buffers.
	gd.m_swapChain->Present(0, 0);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	LPWSTR cmdLine, int cmdShow)
{
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);

	aergiaX::GraphicsDisplay& gd = 
		aergiaX::GraphicsDisplay::create(800, 800, std::string("BlankDemo"), hInstance, cmdShow);
	gd.renderCallback = render;

	so.init(gd.m_d3dDevice);

	return gd.run();
}