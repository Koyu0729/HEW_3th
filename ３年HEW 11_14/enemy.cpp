#include "main.h"
#include "renderer.h"
#include "enemy.h"
#include "modelRenderer.h"
#include "input.h"
#include "camera.h"
#include "manager.h"

// プレイヤーの移動量の変数
constexpr float MOVE_SPEED = 0.1f;

void Enemy::Init()
{
	m_ModelRenderer = new ModelRenderer();
	m_ModelRenderer->Load("asset\\model\\hole4.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	m_Position = Vector3(0.0f, 25.2f, 196.0f);
	m_Rotation = Vector3(0.0f, 0.0f,97.0f);
	m_Scale = Vector3(1.38f, 1.38f, 1.38f);

	//サウンド初期化
	m_params.cueName = "firework";						// Cue名
	m_params.categoryId = SE;                      // SEカテゴリ
	m_params.is3D = true;							// 3D音にする
	m_params.loopCount = CRIATOMPLAYER_FORCE_LOOP;  // ループ再生
	m_params.volume = 1.0f;
	// 先にソースを作成
	m_Source = criAtomEx3dSource_Create(NULL, NULL, 0);

	// ソースを渡して再生開始
	m_SourceId = AudioManager::PlaySeStart(m_params, m_Source);
}

void Enemy::Uninit()
{
	delete m_ModelRenderer;
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Enemy::Update()
{
	static float num = 0.01f;
	static float num2 = 0.3f;
	if (Input::GetKeyPress('I'))
	{
		m_Rotation += Vector3(0.0f, 0.0f, num);
	}
	if (Input::GetKeyPress('O'))
	{
		m_Rotation -= Vector3(0.0f, 0.0f, num);
	}
	//size
	if (Input::GetKeyPress('K'))
	{
		m_Scale.x += num;
		m_Scale.y += num;
		m_Scale.z += num;
	}
	if (Input::GetKeyPress('L'))
	{
		m_Scale.x -= num;
		m_Scale.y -= num;
		m_Scale.z -= num;
	}
	//pos
	if (Input::GetKeyPress('F'))
	{
		m_Position.x += num2;
	}
	if (Input::GetKeyPress('H'))
	{
		m_Position.x -= num2;
	}
	if (Input::GetKeyPress('R'))
	{
		m_Position.y += num2;
	}
	if (Input::GetKeyPress('Y'))
	{
		m_Position.y -= num2;
	}
	if (Input::GetKeyPress('T'))
	{
		m_Position.z += num2;
	}
	if (Input::GetKeyPress('G'))
	{
		m_Position.z -= num2;
	}	
	
	if (Input::GetKeyPress('M'))
	{
		if (Input::GetKeyPress('M'))
		{
			printf("Position: (%.2f, %.2f, %.2f)\n", m_Position.x, m_Position.y, m_Position.z);
			printf("Rotation: (%.2f, %.2f, %.2f)\n", m_Rotation.x, m_Rotation.y, m_Rotation.z);
			printf("Scale:    (%.2f, %.2f, %.2f)\n", m_Scale.x, m_Scale.y, m_Scale.z);
		}
	}

	AudioManager::Update_3DSource_Position(m_Source, m_Position.x, m_Position.y, m_Position.z);
}

void Enemy::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// 3Dマトリクス設定
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y + XM_PI, m_Rotation.z);
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	m_ModelRenderer->Draw();
}

void Enemy::InitSetPosition(Vector3 position)
{
	Enemy::Init();
	SetPosition(position);
}
