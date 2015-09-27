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

	void Shader::createVertexShaderObject(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> vsBlob; // Vertex Shader Blob.

		CHECK_DX_ERROR(D3DReadFileToBlob(L"Build/Debug/VertexShader.cso", &vsBlob);)

			CHECK_DX_ERROR(
				d3dDevice->CreateVertexShader(vsBlob->GetBufferPointer(),
					vsBlob->GetBufferSize(), nullptr, vertexShader.GetAddressOf());
		)
			
			D3D11_INPUT_ELEMENT_DESC inputVertexDataLayout;
		inputVertexDataLayout.SemanticName = "POSITION";
		inputVertexDataLayout.SemanticIndex = 0;
		inputVertexDataLayout.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputVertexDataLayout.InputSlot = 0;
		inputVertexDataLayout.AlignedByteOffset = 0;
		inputVertexDataLayout.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputVertexDataLayout.InstanceDataStepRate = 0;

		CHECK_DX_ERROR(
			d3dDevice->CreateInputLayout(&inputVertexDataLayout, 1,
				vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);
		)
	}

	void Shader::createPixelShaderObject(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> psBlob;

		CHECK_DX_ERROR(
			D3DReadFileToBlob(L"Build/Debug/PixelShader.cso", &psBlob);
		)

			CHECK_DX_ERROR(
				d3dDevice->CreatePixelShader(psBlob->GetBufferPointer(),
					psBlob->GetBufferSize(), nullptr, pixelShader.GetAddressOf());
		)
	}
}