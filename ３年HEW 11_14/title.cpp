#include "main.h"
#include "manager.h"
#include "renderer.h"

//シーン
#include "title.h"

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

FontData m_TitleFont;

void Title::Init()
{
	m_TitleFont.font = L"ZeroGothic";
	m_TitleFont.fontSize = 32.0f;
	m_TitleFont.Color = D2D1::ColorF(D2D1::ColorF::White);
	m_TitleFont.shadowColor = D2D1::ColorF(D2D1::ColorF::Black);
	m_TitleFont.shadowOffset = D2D1::Point2F(2, 2);

	AddGameObject3D<Camera>(0);
	AddGameObject2D<PolyTex>(2)
		->SetTexRule(
			Vector2(0.0f, 0.0f),
			Vector2(0.0f, 0.0f),
			Vector2(SCREEN_WIDTH, SCREEN_HEIGHT),
			"asset\\texture\\test.png"
		);


	auto btnNode = AddGameObject2D<Button>(3);
	btnNode->SetName(L"GameMenu");
	btnNode->SetTexRule(
		Vector2(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 200),
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

	ShowCursor(TRUE);
}

void Title::InitCPU()
{

}
void Title::InitGPU()
{

}

void Title::Uninit()
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


void Title::Update()
{
	Scene::Update();

	Button* button = GetGameObject<Button>(L"GameMenu");
	if (button->IsClicked()|| Input::GetKeyPress('1'))
	{
		Manager::LoadingSetScene<GameMenu>();
	}


}

void Title::Draw()
{
	Scene::Draw();

	Moji::Instance().SetFont(m_TitleFont);
	Moji::Instance().Draw(
		L"タイトル",
		D2D1::Point2F(100, 100),
		2.0f,
		D2D1_DRAW_TEXT_OPTIONS_NONE,
		false
	);
}
//float g_ScrollOffset = 0.0f;
//float wheel = GetMouseWheel();
//g_ScrollOffset += wheel * 10.0f;
//D2D1::Point2F(100, 100 + g_ScrollOffset),