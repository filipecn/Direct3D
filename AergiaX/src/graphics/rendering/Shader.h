#pragma once

#include "common/D3DHeaders.h"

namespace aergiaX {
	class Shader {
	public:
		Shader();
		~Shader();

		// Vertex Shader
		Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
		// Pixel Shader
		Microsoft::WRL::ComPtr<ID3DBlob> psBlob;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;

		void init(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice);
	};
}