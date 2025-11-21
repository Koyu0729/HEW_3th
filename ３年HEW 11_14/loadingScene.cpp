//マネージャーにスレッドを移動した
#include "main.h"
#include "manager.h"
#include "renderer.h"

//シーン
#include "loadingScene.h"

//次のシーン
//#include 特になし

//シーン背景
#include "polyTex.h"

//ロゴ
#include "moji.h"

//UI
#include "camera.h"
#include "input.h"

void LoadingScene::Init()
{
	AddGameObject3D<Camera>(0);

	AddGameObject2D<PolyTex>(0)
		->SetTexRule(
			Vector2(0.0f, 0.0f),
			Vector2(0.0f, 0.0f),
			Vector2(SCREEN_WIDTH, SCREEN_HEIGHT),
			"asset\\texture\\car_handle.png"
		);

	m_LoadingTex = AddGameObject2D<PolyTex>(1)
		->SetTexRule(
			Vector2(SCREEN_WIDTH * 0.9, SCREEN_HEIGHT * 0.9),
			Vector2(0.0f, 0.0f),
			Vector2(200.0f, 200.0f),
			"asset\\texture\\car_handle.png"
		);
}

void LoadingScene::Uninit()
{
	Scene::Uninit();
}

void LoadingScene::Update()
{
	Scene::Update();
	static float num;
	m_LoadingTex->SetRotation(Vector2(0.0f, num));
	num += 0.05f;
}

void LoadingScene::Draw()
{
	Scene::Draw();
}
