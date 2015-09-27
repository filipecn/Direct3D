#include "SceneObject.h"
#include "utils/DebugMessage.hpp"
#include "utils/CheckDxError.hpp"
#include "common/NumericalTypes.hpp"

#include <DirectXMath.h>
#include <d3dcompiler.h>

namespace aergiaX {
	SceneObject::SceneObject() {}

	SceneObject::~SceneObject() {}

	int SceneObject::init(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice) {
		DirectX::XMFLOAT3 vertices[] = {
			DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f),
			DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f),
			DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f),
			DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f)
		};

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.ByteWidth = sizeof(vertices);

		D3D11_SUBRESOURCE_DATA initialData;
		ZeroMemory(&initialData, sizeof(initialData));
		initialData.pSysMem = vertices;

		CHECK_DX_ERROR(
			d3dDevice->CreateBuffer(&vertexBufferDesc, &initialData,
				vertexBuffer.GetAddressOf());
		)

		// Create indices.
		unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };

		// Fill in a buffer description.
		D3D11_BUFFER_DESC indexBufferDesc;
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned int) * 6;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;

		// Define the resource data.
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = indices;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		
		CHECK_DX_ERROR(
		d3dDevice->CreateBuffer(&indexBufferDesc, &InitData, &indexBuffer);
		)
			s.createVertexShaderObject(d3dDevice);
			s.createPixelShaderObject(d3dDevice);
			return 0;
	}

	void SceneObject::render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext) {
		if (!d3dContext) return;

		uint stride = sizeof(DirectX::XMFLOAT3);
		uint offset = 0;

		d3dContext->IASetInputLayout(s.inputLayout.Get());
		d3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		d3dContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
		d3dContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		d3dContext->VSSetShader(s.vertexShader.Get(), nullptr, 0);
		d3dContext->PSSetShader(s.pixelShader.Get(), nullptr, 0);
		d3dContext->DrawIndexed(6, 0, 0);
	}
}

