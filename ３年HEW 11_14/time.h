#pragma once

#include "gameObject.h"

//ゲーム内部の状態管理ならつかいやすいらしいから試し
enum class TimeMode
{
    Countdown,   // スタートのやつ
    Countup      // 経過時間
};

class Time : public GameObject2D
{
private:
    ID3D11Buffer* m_VertexBuffer = nullptr;

    ID3D11InputLayout* m_VertexLayout = nullptr;
    ID3D11VertexShader* m_VertexShader = nullptr;
    ID3D11PixelShader* m_PixelShader = nullptr;
    ID3D11ShaderResourceView* m_Texture = nullptr;

    float m_Time = 0.0f;
    int   m_Value = 0;

    TimeMode m_Mode = TimeMode::Countdown;

public:
    void Init() override;
    void Uninit() override;
    void Update() override;
    void Draw() override;

    void SetTime(int Value) { m_Value += Value; }
    int  GetTime() { return m_Value; }

    void DrawDigit(int num, float x, float y);

    void SetMode(TimeMode mode) { m_Mode = mode; }
    TimeMode GetMode() const { return m_Mode; }


};
