#pragma once

#include "common/D3DHeaders.h"
#include "graphics/rendering/Shader.h"

namespace aergiaX {
	class SceneObject {
	public:
		SceneObject();
		~SceneObject();

		int init(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice);

		void render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext);

		Shader s;

	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	};
}