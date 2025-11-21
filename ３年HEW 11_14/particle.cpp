#include "main.h"
#include "renderer.h"
#include "particle.h"
#include "modelRenderer.h"
#include "input.h"
#include "camera.h"
#include "manager.h"
#include "scene.h"
#include "enemy.h"
#include "texture.h"

// プレイヤーの移動量の変数
constexpr float MOVE_SPEED = 0.1f;

void Particle::Init()
{
	// 頂点バッファの情報を作る
	// 今回は四角形なので４つの頂点情報を入力する必要がある！
	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(-1.0f, 1.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//// テクスチャ読み込み
	//TexMetadata metadata;
	//ScratchImage image;
	//LoadFromWICFile(L"asset\\texture\\Particle.png", WIC_FLAGS_NONE, &metadata, image); // 画像ファイルを読み込む
	//CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(),
	//	image.GetImageCount(), metadata, &m_Texture); // 画像ファイルをセット
	//assert(m_Texture);

	m_Texture = Texture::Load("asset\\texture\\Particle.png");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		m_Particle[i].Enable = false;
	}
}

void Particle::Uninit()
{
	//m_Texture->Release();

	m_VertexBuffer->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Particle::Update()
{

}

void Particle::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	Camera* camera = Manager::GetScene()->GetGameObject<Camera>();

	XMMATRIX view = camera->GetViewMatrix();

	// ビューの逆行列
	XMMATRIX invView;
	invView = XMMatrixInverse(nullptr, view); // 逆行列
	invView.r[3].m128_f32[0] = 0.0f;
	invView.r[3].m128_f32[1] = 0.0f;
	invView.r[3].m128_f32[2] = 0.0f;

	// マテリアル設定
	MATERIAL material{};
	material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	// VertexBufferで生成した情報を使って下さいという意味
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// ブリミティブトボロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 奥行処理をOFF
	Renderer::SetDepthEnable(false);

	// パーティクルの描画
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (m_Particle[i].Enable == true)
		{
			// 3Dマトリクス設定
			XMMATRIX world, scale, trans;
			scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
			trans = XMMatrixTranslation(m_Particle[i].Position.x, m_Particle[i].Position.y, m_Particle[i].Position.z);
			world = scale * invView * trans;
			Renderer::SetWorldMatrix(world);

			// ボリゴン描画
			Renderer::GetDeviceContext()->Draw(4, 0);
		}
	}

	// 奥行処理をON
	Renderer::SetDepthEnable(true);
}

void Particle::FireWork()
{
	int count = 5;

	// 使われていない場所を探す
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (m_Particle[i].Enable == false)
		{
			m_Particle[i].Enable = true;
			m_Particle[i].Life = 600;
			m_Particle[i].Position = m_Position;
			m_Particle[i].Velocity = {};
			count--;
			if (count == 0)
				break;
		}
	}

	// パーティクルの移動
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (m_Particle[i].Enable == true)
		{
			// 速度に加速度を足してから
			// 座標に速度を足す
			if (m_Particle[i].Life >= 300)
			{
				float angle = (rand() % 360) * XM_PI / 180.0f;   // 0～360度
				float speed = (rand() % 100) / 500.0f + 0.05f;   // 0.05～0.25くらい


				m_Particle[i].Position = m_Position;
				m_Particle[i].Velocity = Vector3(cosf(angle) * speed,
					sinf(angle) * speed,
					0.0f);
			}

			// ライフを引く
			m_Particle[i].Life--;

			// ライフが０になったら消す
			if (m_Particle[i].Life <= 0)
			{
				m_Particle[i].Enable = false;
			}
		}
	}
}

void Particle::DefUP()
{
	int count = 5;

	// 使われていない場所を探す
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (m_Particle[i].Enable == false)
		{
			m_Particle[i].Enable = true;
			m_Particle[i].Life = 60;
			m_Particle[i].Position = m_Position;
			m_Particle[i].Velocity = Vector3((rand() % 100 - 50 / 500.0f) / 100.0f,
				(rand() % 100 + 50 / 500.0f) / 100.0f,
				0.0f/*(rand() % 100 - 50 / 500.0f) / 100.0f*/);
			count--;
			if (count == 0)
				break;
		}
	}

	// パーティクルの移動
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (m_Particle[i].Enable == true)
		{
			// 速度に加速度を足してから
			// 座標に速度を足す
			m_Particle[i].Velocity += Vector3(0.0f, -0.01f, 0.0f);
			m_Particle[i].Position += m_Particle[i].Velocity;

			// ライフを引く
			m_Particle[i].Life--;

			// ライフが０になったら消す
			if (m_Particle[i].Life <= 0)
			{
				m_Particle[i].Enable = false;
			}
		}
	}
}