#pragma once

#include "gameObject.h"

class PolyTex : public GameObject2D
{
private:
	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout = nullptr;
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;

	ID3D11ShaderResourceView* m_Texture = nullptr;

	XMFLOAT4 m_Color = { 1,1,1,1 };
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	PolyTex* SetTexRule(
		Vector2 position = Vector2(0.0f, 0.0f),
		Vector2 rotation = Vector2(0.0f, 0.0f),
		Vector2 scale = Vector2(0.0f, 0.0f),
		const char* FileName = "asset\\texture\\test.png"
	);

	void SetColor(const XMFLOAT4& color) { m_Color = color; }

	bool Inside2D();
};