#pragma once
#include "scene.h"

class LoadingScene : public Scene
{
private:
    GameObject2D* m_LoadingTex = nullptr;
public:
    void Init() override;
    void Update() override;
    void Draw() override;
    void Uninit() override;
};