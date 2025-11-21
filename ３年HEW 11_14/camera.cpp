#include "main.h"
#include "renderer.h"
#include "camera.h"
#include "player.h"
#include "manager.h"
#include "scene.h"
#include "input.h"

void Camera::Init()
{
	m_Position = { 0.0f, 1.0f, -5.0f };
}

void Camera::Uninit()
{
}

//void Camera::Update()
//{
//	Player* player = Manager::GetScene()->GetGameObject<Player>();
//	m_Target = player->GetPosition() + Vector3(0.0f, 1.0f, 0.0f);
//
//
//	const float moveSpeed = 1.00f; // 感度調整（小さくしないと暴れる）
//	printf("[Cam] g_mouseX=%ld g_mouseY=%ld  before pos=(%f,%f,%f)\n",
//		g_mouseX, g_mouseY, m_Position.x, m_Position.y, m_Position.z);
//	m_Position.x += (float)g_mouseX * moveSpeed; // 左右
//	m_Position.y += (float)g_mouseY * moveSpeed; // 上下
//	m_Position.z += (float)g_mouseY * moveSpeed;
//
//
//	printf("pos X=%f Y=%f\n", m_Position.x, m_Position.y);
//	//m_Position = m_Target + 
//	//	Vector3(-sinf(m_Rotation.y), 0.5f, -cosf(m_Rotation.y)) * 5.0f;
//}

//https://stackoverflow.com/questions/51341126
void Camera::Update()
{
	float offset_x = (float)g_mouseX;
	float offset_y = (float)g_mouseY;
    static float yaw = 0.0f;   // 左右角度
	static float pitch = 0.0f; // 上下角度

	Player* player = Manager::GetScene()->GetGameObject<Player>();
	if (!player) return;


	Vector3 pos = player->GetPosition();
	Vector3 rot = player->GetRotation();

	// プレイヤーの頭の位置を注視点に
	m_Target = pos + Vector3(0.0f, 1.0f, 0.0f);

	//前方方向から３０度までの制限
	//m_Target = player->GetRotation();

	Vector3 back(
		-sinf(rot.y),   // X
		0.3f,           // Y = 少し上から
		-cosf(rot.y)    // Z
	);

	float distance = 6.0f; // 車との距離（調整可）
	m_Position = m_Target + back * distance;
}


void Camera::Draw()
{
	// プロジェクションマトリクス
	m_Projection = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(m_Projection);

	// ビューマトリクス
	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_View = XMMatrixLookAtLH(XMLoadFloat3((XMFLOAT3*)&m_Position),
		XMLoadFloat3((XMFLOAT3*)&m_Target),
		XMLoadFloat3(&up));

	Renderer::SetViewMatrix(m_View);
}
