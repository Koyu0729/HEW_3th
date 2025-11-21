#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

#include "scene.h"
#include "title.h"
#include "game.h"

ManagerState Manager::m_State = ManagerState::Default;

Scene* Manager::m_Scene = nullptr;
Scene* Manager::m_SceneNext = nullptr;
Fade Manager::m_Fade;

std::function<void()> Manager::m_Thread = nullptr;

//ロード用
std::atomic<bool> Manager::m_Loading{ false };
std::thread Manager::m_LoadThread;

//ノード関連
std::vector<NodeBase*> Manager::m_RunNodes;

void Manager::Init()
{
	Renderer::Init();
	Input::Init();

	m_Fade.Init();  // フェード初期化

	m_Scene = new Title();
	m_Scene->Init();
}


void Manager::Uninit()
{
	if (m_LoadThread.joinable()) {
		m_LoadThread.join(); // スレッド終了待ち
	}

	m_Scene->Uninit();
	delete m_Scene;

	m_Fade.Uninit();

	Input::Uninit();
	Renderer::Uninit();

}

void Manager::Update()
{
	Input::Update();
	m_Scene->Update();
	m_Fade.Update();

	switch (m_State)
	{
	case ManagerState::Default:
		//今は、使わない
		break;

	case ManagerState::CreateLoadScene:

		if (m_Fade.IsFinished() && m_Fade.GetState() == Fade::FadeOut)
		{
			// 今のシーンを消して、ローディングシーンへ
			if (m_Scene)
			{
				m_Scene->Uninit();
				delete m_Scene;
			}

			m_Scene = new LoadingScene();
			m_Scene->Init();

			m_Fade.StartFadeIn(1.0f);
			m_State = ManagerState::LoadingScene;

			m_Thread();
		}
		break;

	case ManagerState::LoadingScene:

		if (!m_Loading && m_SceneNext && m_Fade.GetState() == Fade::None)
		{
			m_SceneNext->InitGPU();
			m_Fade.StartFadeOut(1.0f);
			m_State = ManagerState::ChangeScene;
		}
		break;

	case ManagerState::ChangeScene:

		if (m_Fade.IsFinished() && m_Fade.GetState() == Fade::FadeOut)
		{
			// ロード完了後、実際にシーン切り替え
			if (m_Scene)
			{
				m_Scene->Uninit();
				delete m_Scene;
			}

			m_Scene = m_SceneNext;
			m_SceneNext = nullptr;

			m_Fade.StartFadeIn(1.0f);
			m_State = ManagerState::Default;
		}
		break;
	}

	//マウス確認
	
	//if (g_MouseLeftDown) printf("ホールド開始\n");
	//if (g_MouseLeftUp)   printf("ホールド解除\n");
}

void Manager::Draw()
{
	// これは一番前に持ってくる！
	Renderer::Begin();

	m_Scene->Draw();

	// フェードを一番上に描画
	m_Fade.Draw();

	// これは一番下に持ってくる！
	Renderer::End();
}