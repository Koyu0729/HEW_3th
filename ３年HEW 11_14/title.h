#pragma once

#include "scene.h"

class Title : public Scene
{
public:
	void Init() override;
	void InitCPU() override;
	void InitGPU() override;

	void Uninit() override;
	void Update() override;
	void Draw() override;

	//ADX用
	SePlayParams m_params;                 // 再生パラメータ
	CriAtomEx3dSourceHn m_Source = nullptr; // 3Dソースハンドル
	CriAtomExPlaybackId m_SourceId = -1;    // 再生ID
};