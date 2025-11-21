//#include "main.h"
//#include "renderer.h"
//#include "bullet.h"
//#include "modelRenderer.h"
//#include "input.h"
//#include "camera.h"
//#include "manager.h"
//#include "scene.h"
//#include "enemy.h"
//#include "explosion.h"
//
//// プレイヤーの移動量の変数
//constexpr float MOVE_SPEED = 0.1f;
//
//void Bullet::Init()
//{
//	m_ModelRenderer = new ModelRenderer();
//	m_ModelRenderer->Load("asset\\model\\bullet.obj");
//
//	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
//		"shader\\unlitTextureVS.cso");
//
//	Renderer::CreatePixelShader(&m_PixelShader,
//		"shader\\unlitTexturePS.cso");
//}
//
//void Bullet::Uninit()
//{
//	delete m_ModelRenderer;
//	m_VertexLayout->Release();
//	m_VertexShader->Release();
//	m_PixelShader->Release();
//}
//
//void Bullet::Update()
//{
//	// 移動
//	//m_Position += m_Velocity;
//
//	// エルミート曲線で移動
//	m_Time += 0.01f;
//	float i = m_Time;
//
//	m_Position = m_StartPosition * (2.0f * i * i * i - 3.0f * i * i + 1.0f) +
//				m_EndPosition * (-2.0f * i * i * i + 3 * i * i) +
//				m_StartVector * (i * i * i - 2 * i * i + i) +
//				m_EndVector * (i * i * i - i * i);
//
//	if (m_Position.length() > 10.0f)
//	{
//		SetDestory();
//	}
//
//	std::vector<Enemy*> enemys = Manager::GetScene()->GetGameObjects<Enemy>();
//
//	for (auto enemy : enemys)
//	{
//		// 敵との衝突判定
//		Vector3 d = m_Position - enemy->GetPosition();
//		float length = d.length();
//
//		// 半径１以内だったら
//		if (length < 1.0f)
//		{
//			Manager::GetScene()->AddGameObject<Explosion>(1)->SetPosition(enemy->GetPosition() + Vector3(0.0f, 1.0f, 0.0f));
//			
//			// 当たっていたら消す
//			enemy->SetDestory();
//			this->SetDestory();
//		}
//	}
//
//	Manager::GetScene()->AddGameObject<Explosion>(1)->SetPosition(m_Position + Vector3(0.0f, 1.0f, 0.0f));
//}
//
//void Bullet::Draw()
//{
//	//入力レイアウト設定
//	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
//
//	//シェーダ設定
//	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
//	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);
//
//	// 3Dマトリクス設定
//	XMMATRIX world, scale, rot, trans;
//	scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
//	rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y + XM_PI, m_Rotation.z);
//	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
//	world = scale * rot * trans;
//	Renderer::SetWorldMatrix(world);
//
//	m_ModelRenderer->Draw();
//}
//
