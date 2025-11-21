//ノードをつけたキャラクターが動く場所（fieldが変わってもキャラクターが動くのはここになる）
#include "main.h"
#include "manager.h"
#include "renderer.h"

//シーン
#include "game.h"

//次のシーン
#include "result.h"

//シーン背景
#include "polyTex.h"

//ロゴ
#include "moji.h"

//UI
#include "camera.h"
#include "input.h"
#include "button.h"

//ゲームオブジェクト
#include "player.h"
#include "field.h"
#include "enemy.h"
#include "score.h"
#include "meshField.h"
#include "particle.h"
#include "sky.h"
#include "time.h"

void Game::Init()
{	
	AddGameObject3D<Camera>(0)->SetName(L"MainCamera");
	//AddGameObject<Sky>(1);
	AddGameObject3D<Field>(2);
	AddGameObject3D<Player>(3);

	AddGameObject2D<Time>(0);

	ShowCursor(FALSE);
}

void Game::InitCPU()
{
	//AddGameObject3D<MeshField>(3)->InitCPU();
	
	// ダミー負荷：約数億回の演算でCPUを使わせる
	//volatile double dummy = 0;
	//for (int i = 0; i < 500000000; ++i) {
	//	dummy += std::sin(i) * std::cos(i);
	//}
}

void Game::InitGPU()
{
	//GetGameObject<MeshField>()->InitGPU();
}

void Game::Uninit()
{
	Scene::Uninit();


}

void Game::Update()
{
	Scene::Update();
}

void Game::Draw()
{
	Scene::Draw();
}