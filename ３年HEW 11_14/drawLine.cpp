#include "main.h"
#include "drawLine.h"
#include "renderer.h"

void DrawLine::Init()
{
	VERTEX_3D vertex[50] = {};

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 50;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");
}

void DrawLine::Uninit()
{
	m_VertexBuffer->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void DrawLine::Update()
{
	if (!m_VertexBuffer) return;
	int loop = 0;
	Vector2 pos;

	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	for (float i = 0; i < 50; i++)
	{
		float t = i / float(50 - 1);

		pos.x = (1.0f - t) * (1.0f - t) * (1.0f - t) * m_P0.x
			+ 3.0f * (1.0f - t) * (1.0f - t) * t * m_P1.x
			+ 3.0f * (1.0f - t) * t * t * m_P2.x
			+ t * t * t * m_P3.x;

		pos.y = (1.0f - t) * (1.0f - t) * (1.0f - t) * m_P0.y
			+ 3.0f * (1.0f - t) * (1.0f - t) * t * m_P1.y
			+ 3.0f * (1.0f - t) * t * t * m_P2.y
			+ t * t * t * m_P3.y;


		vertex[loop].Position = XMFLOAT3(pos.x, pos.y, 0.0f);
		vertex[loop].Diffuse = m_Color;
		loop++;
	}

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
}

void DrawLine::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);
	// マトリクス設定
	Renderer::SetWorldViewProjection2D();

	// マテリアル設定
	MATERIAL material{};
	material.Diffuse = m_Color;
	material.TextureEnable = false;
	Renderer::SetMaterial(material);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(50, 0);
}

void DrawLine::SetPoints(const Vector2& p0, const Vector2& p3)
{
    m_P0 = p0;
	m_P1 = Vector2(p0.x, p3.y);
	m_P2 = Vector2(p3.x, p0.y);
    m_P3 = p3;
}

void DrawLine::SetColor(const XMFLOAT4& color)
{
    m_Color = color;
}


