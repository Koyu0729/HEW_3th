#pragma once

#include "gameObject.h"


class Score : public GameObject2D
{
private:
	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayOut;
	ID3D11PixelShader* m_PixelShader;
	ID3D11VertexShader* m_VertexShader;

	ID3D11ShaderResourceView* m_Texture;

	int m_Value;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void Add(int value) { m_Value += value; }
};
