#pragma once

#include <DirectXMath.h>
#include "graphics/scene/SceneObject.h"

class Cube : public aergiaX::SceneObject {
	
	ID3D11Buffer* viewCB_; 
	ID3D11Buffer* projCB_; 
	ID3D11Buffer* worldCB_; 

	DirectX::XMMATRIX viewMatrix_; 
	DirectX::XMMATRIX projMatrix_;

public:
	virtual int init(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice) override;

	virtual void render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext) override;
};