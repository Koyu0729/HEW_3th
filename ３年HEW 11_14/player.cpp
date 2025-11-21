#include "main.h"
#include "renderer.h"
#include "player.h"
#include "modelRenderer.h"
#include "input.h"
#include "camera.h"
#include "manager.h"
#include "scene.h"
#include "meshField.h"

#include "time.h"
#include "result.h"

//ノード関連
#include "nodeBase.h"
#include "nodeRuntime.h"

#define MOVE_SPEED (0.1f) 

NodeRuntime* g_Run = nullptr;

void Player::Init()
{
	m_ModelRenderer = new ModelRenderer();
	m_ModelRenderer->Load("asset\\model\\player.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	SetScale(Vector3(0.3f, 0.3f, 0.3f));

	m_Position = Vector3(0.0f, 1.5f, -100.0f);

#pragma region ノード
	g_Run = new NodeRuntime();

	for (auto* node : Manager::GetRuntimeNodes())
	{
		node->SetPlayer(this);
	}
#pragma endregion
	//サウンド初期化
	m_params.cueName = "bgm";						// Cue名
	m_params.categoryId = BGM;                      // SEカテゴリ
	m_params.is3D = false;							// 3D音にする
	m_params.loopCount = CRIATOMPLAYER_FORCE_LOOP;  // ループ再生
	m_params.volume = 0.0f;
	// 先にソースを作成
	m_Source = criAtomEx3dSource_Create(NULL, NULL, 0);

	// ソースを渡して再生開始
	m_SourceId = AudioManager::PlaySeStart(m_params, m_Source);
}

void Player::Uninit()
{

	delete g_Run;
	g_Run = nullptr;

	delete m_ModelRenderer;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Player::Update()
{
	Camera* camera = Manager::GetScene()->GetGameObject<Camera>();

	// カメラの向いている方向を受け取る
	Vector3 rotation = camera->GetRotation();
	// カメラが向いている方向をプレイヤーが向く
	m_Rotation.y = rotation.y;


	if (Input::GetKeyPress('0'))
	{
		const auto& nodes = Manager::GetRuntimeNodes();

		if (nodes.empty())
		{
			printf("実行ノードがありません\n");
		}
		else
		{
			g_Run->Run(nodes);
		}
	}

	if (m_Stats.hp <= 0 || m_Stats.fuel < 0)
	{
		Manager::LoadingSetScene<Result>();
	}
	else
	{
		m_Stats.fuel--;
	}


	//３D音響：更新
	AudioManager::Update_3DListener_Position(m_Position.x, m_Position.y, m_Position.z);

	CriAtomExVector forward = { sinf(m_Rotation.y), m_Rotation.y,  cosf(m_Rotation.y) };
	CriAtomExVector top = { 0.0f, 1.0f, 0.0f };

	AudioManager::Config3DListener_Orientation(forward, top);
}

void Player::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// 3Dマトリクス設定
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_Scale.x, m_Scale.y,m_Scale.z);
	rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y/* + XM_PI*/, m_Rotation.z);
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	m_ModelRenderer->Draw();
}