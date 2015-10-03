#include <iostream>

#include "utils/debug/Console.h" 
#include "io/GraphicsDisplay.h"
#include "Grid.h"
#include "UGrid.h"

#include<Windows.h>


void render() {
	aergiaX::GraphicsDisplay& gd = aergiaX::GraphicsDisplay::instance();
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

	Console::instance();
	Grid<double> g(10, 10);
	g(2, 2) = 3.0;

	UGrid<double> ug(-1.0,1.0,-1.0,1.0);

	for (int j = g.size[1] - 1; j >= 0; j--) {
		for (int i = 0; i < g.size[0]; i++) {
			double v = g(i, j);
			std::cout << g(i, j) << " ";
		}
		std::cout << std::endl;
	}

	aergiaX::GraphicsDisplay& gd =
		aergiaX::GraphicsDisplay::create(800, 800, std::string("FLIPDemo"), hInstance, cmdShow);
	gd.renderCallback = render;

	return gd.run();
}