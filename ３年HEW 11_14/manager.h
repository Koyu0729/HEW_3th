//ver1.1　変更点：すべてのシーン遷移にLoadingSceneを挟むことで、プレイヤーを退屈させないよう改造
#pragma once
#include <thread>   //別スレッド用
#include <atomic>   //複数のスレッドが同じ変数にアクセスしても安全に値を読み書きできるようにするよう
#include "fade.h"

#include "scene.h"
#include "loadingScene.h"

//ノード情報の保存
#include "nodeBase.h"
#include "button.h"

#include "kosuCRI.h" 

enum class ManagerState
{
	Default,
	CreateLoadScene,	//ロードシーン移動
	LoadingScene,		//CPUロード処理中
	ChangeScene			//シーン移動
};
// クラスは関数を入れることができる！
class Manager
{
private:

	static ManagerState m_State;

	static class Scene* m_Scene; // 現在のシーンを格納する
	static class Scene* m_SceneNext;
	static Fade m_Fade;

	static std::atomic<bool> m_Loading;
	static std::thread m_LoadThread;

	static std::function<void()> m_Thread;

	static std::vector<NodeBase*> m_RunNodes;
public:
	static void Init(); // 初期化
	static void Uninit(); // 終了
	static void Update(); // 更新
	static void Draw(); // 描画

	//残しておく
	//template <typename T>
	//static void SetScene()
	//{
	//	m_SceneNext = new T();
	//}

	//ロードをシーンを挟むように改造
	template <typename T>
	static void LoadingSetScene()
	{
		if (m_LoadThread.joinable())
		{
			m_LoadThread.join();
			m_Loading = false;
		}

		m_Thread = []()
			{
				m_Loading = true;
				m_LoadThread = std::thread([]()
					{
						printf("ロード開始\n");
						Scene* next = new T();
						next->Init();
						next->InitCPU();
						m_SceneNext = next;
						m_Loading = false;
						printf("ロード完了\n");
					});
			};
		m_Fade.StartFadeOut(1.0f);
		m_State = ManagerState::CreateLoadScene;
	}	

	//Setter
	static void SetRuntimeNodes(const std::vector<NodeBase*>& nodes)
	{
		m_RunNodes = nodes;
	}

	//ノードマネージャのゲッター（ノードエディターで作った接続情報をいれる）
	static const std::vector<NodeBase*>& GetRuntimeNodes()
	{
		return m_RunNodes;
	}

	static Scene* GetScene() { return m_Scene; }
};