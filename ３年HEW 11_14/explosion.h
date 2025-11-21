#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "gameObject.h"

class Explosion : public GameObject3D // 継承
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
	int m_Frame;
	
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};

#endif // BULLET_H
