#ifndef ENEMY_H
#define ENEMY_H

#include "gameObject.h"
#include "kosuCRI.h" 
class ModelRenderer;

class Enemy : public GameObject3D // 継承
{
private:
	// 頂点バッファ→GPUに送るためやつ
	ID3D11Buffer* m_VertexBuffer = NULL;
	// 
	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	class ModelRenderer* m_ModelRenderer;

	//ADX用
	SePlayParams m_params;                 // 再生パラメータ
	CriAtomEx3dSourceHn m_Source = nullptr; // 3Dソースハンドル
	CriAtomExPlaybackId m_SourceId = -1;    // 再生ID
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void InitSetPosition(Vector3 position);
};

#endif // ENEMY_H
