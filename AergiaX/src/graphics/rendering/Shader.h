#pragma once

#include "common/D3DHeaders.h"

namespace aergiaX {
	class Shader {
	public:
		Shader();
		~Shader();

		Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;

		Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	
	//protected:
		void createVertexShaderObject(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice);
		void createPixelShaderObject(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice);
	};
}