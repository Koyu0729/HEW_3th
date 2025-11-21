#pragma once

#include "scene.h"

class PlayerTest : public Scene
{
private:
	class Audio* m_BGM;
public:
	void Init() override;
	void InitCPU() override;
	void InitGPU() override;

	void Uninit() override;
	void Update() override;
	void Draw() override;
};