#include "main.h"
#include "manager.h"
#include "renderer.h"

//シーン
#include "gameMenu.h"

//次のシーン(分岐する)
#include "nodeEditor.h"
#include "playerTest.h"
//#include "playerCustom.h"
#include "game.h"

//シーン背景
#include "polyTex.h"

//ロゴ
#include "moji.h"

//UI
#include "camera.h"
#include "input.h"
#include "button.h"

void GameMenu::Init()
{
	AddGameObject3D<Camera>(0);
	AddGameObject2D<PolyTex>(2)
		->SetTexRule(
			Vector2(0.0f, 0.0f),
			Vector2(0.0f, 0.0f),
			Vector2(SCREEN_WIDTH, SCREEN_HEIGHT),
			"asset\\texture\\wallpaper\\editor.jpg"
		);

	auto btnNode = AddGameObject2D<Button>(3);
	btnNode->SetName(L"NodeEditor");
	btnNode->SetTexRule(
		Vector2(SCREEN_WIDTH / 4, SCREEN_HEIGHT - 200),
		Vector2(0.0f, 0.0f),
		Vector2(200.0f, 100.0f),
		"asset\\texture\\grass.jpg"
	);


	auto btnTest = AddGameObject2D<Button>(3);
	btnTest->SetName(L"PlayerTest");
	btnTest->SetTexRule(
		Vector2((SCREEN_WIDTH / 4) * 2, SCREEN_HEIGHT - 200),
		Vector2(0.0f, 0.0f),
		Vector2(200.0f, 100.0f),
		"asset\\texture\\grass.jpg"
	);

	auto btnGame = AddGameObject2D<Button>(3);
	btnGame->SetName(L"Game");
	btnGame->SetTexRule(
		Vector2(Vector2((SCREEN_WIDTH / 4) * 3, SCREEN_HEIGHT - 200)),
		Vector2(0.0f, 0.0f),
		Vector2(200.0f, 100.0f),
		"asset\\texture\\grass.jpg"
	);
	//サウンド初期化
	m_params.cueName = "bgm";						// Cue名
	m_params.categoryId = BGM;                      // SEカテゴリ
	m_params.is3D = false;							// 3D音にする
	m_params.loopCount = CRIATOMPLAYER_FORCE_LOOP;  // ループ再生
	m_params.volume = 0.1f;

	// 先にソースを作成
	m_Source = criAtomEx3dSource_Create(NULL, NULL, 0);

	// ソースを渡して再生開始
	m_SourceId = AudioManager::PlaySeStart(m_params, m_Source);
}

void GameMenu::InitCPU()
{

}
void GameMenu::InitGPU()
{

}

void GameMenu::Uninit()
{
	// BGM停止（即時停止）
	if (m_SourceId != -1)
	{
		AudioManager::StopPlayback(m_SourceId, true);
		m_SourceId = -1;
	}

	// ソース破棄
	if (m_Source)
	{
		criAtomEx3dSource_Destroy(m_Source);
		m_Source = nullptr;
	}

	Scene::Uninit();
}

void GameMenu::Update()
{
	Scene::Update();
	
	Button* buttonNodeEditor = GetGameObject<Button>(L"NodeEditor");
	if (buttonNodeEditor && buttonNodeEditor->IsClicked())
	{
		Manager::LoadingSetScene<NodeEditor>();
	}

	Button* buttonPlayerTest = GetGameObject<Button>(L"PlayerTest");
	if (buttonPlayerTest && buttonPlayerTest->IsClicked())
	{
		Manager::LoadingSetScene<PlayerTest>();
	}

	//Button* button = GetGameObject<Button>(L"StartButton");
	//if (button->IsClicked() || Input::GetKeyPress('1'))
	//{
	//	Manager::LoadingSetScene<GameMenu>();
	//}

	Button* buttonGame = GetGameObject<Button>(L"Game");
	if (buttonGame && buttonGame->IsClicked())
	{
		Manager::LoadingSetScene<Game>();
	}

}

void GameMenu::Draw()
{
	Scene::Draw();

}
