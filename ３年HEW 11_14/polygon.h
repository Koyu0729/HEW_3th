#ifndef POLYGON_H
#define POLYGON_H

#include "gameObject.h"
#include <d3d11.h>

class Polygon : public GameObject
{
private:
	// ポリゴンを描画するのに必要な４つ！

	// 頂点バッファ→GPUに送るためやつ
	ID3D11Buffer * m_VertexBuffer = NULL;
	// 
	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};

#endif // POLYGON_H
