#include "Shader.h"
#include "utils/DebugMessage.hpp"
#include "utils/CheckDxError.hpp"
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#pragma warning(disable:4996)
#include "utils\debug\Console.h"
#include <d3dcompiler.h>
#include <cstdio>
namespace aergiaX {
	Shader::Shader() {}
	Shader::~Shader() {}

	void Shader::init(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice) {
		
		// init vertex shader
		CHECK_DX_ERROR(
			D3DReadFileToBlob(L"Build/Debug/VertexShader.cso", &vsBlob);
		)

		CHECK_DX_ERROR(
				d3dDevice->CreateVertexShader(vsBlob->GetBufferPointer(),
					vsBlob->GetBufferSize(), nullptr, vertexShader.GetAddressOf());
		)

		// init pixel shader

		CHECK_DX_ERROR(
				D3DReadFileToBlob(L"Build/Debug/PixelShader.cso", &psBlob);
		)

		CHECK_DX_ERROR(
				d3dDevice->CreatePixelShader(psBlob->GetBufferPointer(),
					psBlob->GetBufferSize(), nullptr, pixelShader.GetAddressOf());
		)
	}
}