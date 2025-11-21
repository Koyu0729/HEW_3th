#include "main.h"
#include "manager.h"
#include "renderer.h"

//シーン
#include "result.h"

//次のシーン
#include "gameMenu.h"

//シーン背景
#include "polyTex.h"

//ロゴ
#include "moji.h"

//UI
#include "camera.h"
#include "input.h"
#include "button.h"

void Result::Init()
{
	AddGameObject3D<Camera>(0);

	AddGameObject2D<PolyTex>(2)
		-> SetTexRule(
			Vector2(0.0f, 0.0f),
			Vector2(0.0f, 0.0f),
			Vector2(SCREEN_WIDTH, SCREEN_HEIGHT),
			"asset\\texture\\wallpaper\\result.png");

	AddGameObject2D<Button>(3)->SetName(L"StartButton");
}

void Result::Uninit()
{
	Scene::Uninit();
}

void Result::Update()
{
	Button* button = GetGameObject<Button>(L"StartButton");
	if (button->IsClicked())
	{
		Manager::LoadingSetScene<GameMenu>();
	}
}

void Result::Draw()
{
	Scene::Draw();
}
