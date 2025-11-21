#ifndef BULLET_H
#define BULLET_H

#include "gameObject.h"

class ModelRenderer;

class Bullet : public GameObject // 継承
{
private:
	// ポリゴンを描画するのに必要な４つ！

	// 頂点バッファ→GPUに送るためやつ
	ID3D11Buffer* m_VertexBuffer = NULL;
	// 
	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	class ModelRenderer* m_ModelRenderer;

	Vector3 m_Velocity = {};

	Vector3 m_StartPosition = {};
	Vector3 m_EndPosition = {};
	Vector3 m_StartVector = {};
	Vector3 m_EndVector = {};
	float m_Time;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetVelocity(Vector3 velocity) { m_Velocity = velocity; }
	Vector3 GetVelocity() { return m_Velocity; }

	void Shot(Vector3 startposition, Vector3 EndPosition)
	{
		m_StartPosition = startposition;
		m_EndPosition = EndPosition;
		m_StartVector = Vector3(2.0f, 2.0f, 0.0f);
		m_EndVector = Vector3(2.0f, -2.0f, 0.0f);
		m_Time = 0.0f;
	}
};

#endif // BULLET_H
