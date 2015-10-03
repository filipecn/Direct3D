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
		
		// Vertex Buffer
		{
			DirectX::XMFLOAT3 vertices[] = {
				DirectX::XMFLOAT3(-0.05f, -0.05f, 1.0f),
				DirectX::XMFLOAT3(-0.05f, 0.05f, 0.0f),
				DirectX::XMFLOAT3(0.05f, 0.05f, 0.0f),
				DirectX::XMFLOAT3(0.05f, -0.05f, 0.0f)
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
		}

		// Index Buffer
		{
			unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };

			// Fill in a buffer description.
			D3D11_BUFFER_DESC indexBufferDesc;
			ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
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
		}

		// Constant Buffer
		{

		}

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			// Data from the vertex buffer
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			//{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			//{ "NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },

			// Data from the instance buffer
			{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32_FLOAT,    1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
			//{ "INSTANCECOLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,    1, 12, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
		};

		D3D11_INPUT_ELEMENT_DESC inputVertexDataLayout;
		inputVertexDataLayout.SemanticName = "POSITION";
		inputVertexDataLayout.SemanticIndex = 0;
		inputVertexDataLayout.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputVertexDataLayout.InputSlot = 0;
		inputVertexDataLayout.AlignedByteOffset = 0;
		inputVertexDataLayout.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputVertexDataLayout.InstanceDataStepRate = 0;

		s.init(d3dDevice);

		CHECK_DX_ERROR(
			d3dDevice->CreateInputLayout(&inputVertexDataLayout, 1,
				s.vsBlob->GetBufferPointer(), s.vsBlob->GetBufferSize(), &inputLayout);
		)
			
			return 0;
	}

	void SceneObject::render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext) {
		if (!d3dContext) return;

		uint stride = sizeof(DirectX::XMFLOAT3);
		uint offset = 0;
		
		d3dContext->IASetInputLayout(inputLayout.Get());
		d3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		d3dContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
		d3dContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		d3dContext->VSSetShader(s.vertexShader.Get(), nullptr, 0);
		d3dContext->PSSetShader(s.pixelShader.Get(), nullptr, 0);
		d3dContext->DrawIndexedInstanced(6, 72, 0, 0, 0);
	}
}

