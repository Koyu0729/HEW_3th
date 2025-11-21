#ifndef PARTICLE_H
#define PARTICLE_H

#include "gameObject.h"
#include "vector3.h"

class Particle : public GameObject3D // 継承
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

	struct PARTICLE
	{
		bool Enable;		// 使われているか
		int Life;			// 寿命
		Vector3 Position;	// 座標
		Vector3 Velocity;	// 速度
	};
	
	static const int PARTICLE_MAX = 10000;
	PARTICLE m_Particle[PARTICLE_MAX];
	// std::arrayの方が安全性が高い

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void FireWork();
	void DefUP();
};

#endif // BULLET_H
