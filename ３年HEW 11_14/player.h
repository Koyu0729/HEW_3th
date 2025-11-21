#ifndef PLAYER_H
#define PLAYER_H

#include "gameObject.h"
#include "kosuCRI.h" 
class ModelRenderer;

class Player : public GameObject3D // 継承
{
private:
	// Ray用データ
	Vector3 m_Forward;      // ローテーションから計算された前方ベクトル
	Vector3 m_EyePosition;

	struct Stats
	{
		int hp = 100;			//HP
		int fuel = 100;			//制限時間

		float speed = 0.1f;		//移動速度
		float moveStable = 1.0f;//移動速度係数

		float aimSpeed = 0.05f;	//標準が合うまでのスピード
		float armStable = 1.0f;	//標準が合うまでの係数

		GameObject3D* findObject = nullptr;	//ノード側からセットする
		bool Shield = false;
	};

	Stats m_Stats;

	// 頂点バッファ→GPUに送るためやつ
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	class ModelRenderer* m_ModelRenderer;

public:
	//ADX用
	SePlayParams m_params;                 // 再生パラメータ
	CriAtomEx3dSourceHn m_Source = nullptr; // 3Dソースハンドル
	CriAtomExPlaybackId m_SourceId = -1;    // 再生ID
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	const Player* GetPlayer() const { return this; }
	const Stats* GetStats() const { return &m_Stats; }

	int GetHP() const { return m_Stats.hp; }
	float GetMoveSpeed() const { return m_Stats.speed; }

	void SetFindObject(GameObject3D* findObject) { m_Stats.findObject = findObject;}
	GameObject3D* GetFindObject() const{ return m_Stats.findObject; }
	
};

#endif // PLAYER_H
