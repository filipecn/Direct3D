#pragma once

#include "common/D3DHeaders.h"
#include "graphics/rendering/Shader.h"

namespace aergiaX {
	class SceneObject {
	public:
		SceneObject();
		~SceneObject();

		virtual int init(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice);

		virtual void render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext);

		Shader s;

	protected:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> instanceBuffer;
	};
}