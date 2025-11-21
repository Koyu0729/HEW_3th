#ifndef FILED_H
#define FILED_H

#include "gameObject.h"

class Field : public GameObject3D
{
private:
	// ポリゴンを描画するのに必要な４つ！

	// 頂点バッファ→GPUに送るためやつ
	ID3D11Buffer* m_VertexBuffer = NULL;
	// 
	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture;

	XMFLOAT3 m_Pos;
	XMFLOAT2 m_Size;
	float m_Rad;
	XMFLOAT4 m_Color;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};

#endif // FILED_H
