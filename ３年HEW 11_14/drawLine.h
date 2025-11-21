//p0(始点)とp3（終点）がポート（またはマウス座標）の頂点なのはわかる
//p1とp2はp0のxとp3のy,p3のxとp0のy。これでいいや。
//GYのパクリ
#pragma once
#include "gameObject.h"

// ベジェ曲線を描画するクラス
class DrawLine : public GameObject2D
{
private:
    ID3D11Buffer* m_VertexBuffer;

    ID3D11InputLayout* m_VertexLayout;
    ID3D11VertexShader* m_VertexShader;
    ID3D11PixelShader* m_PixelShader;

    Vector2 m_P0 ={}; // 始点
    Vector2 m_P1 ={}; // 制御点1
    Vector2 m_P2 ={}; // 制御点2
    Vector2 m_P3 ={}; // 終点

    XMFLOAT4 m_Color = { 0,0,0,1 };

public:
    void Init() override;
    void Uninit() override;
    void Update() override;
    void Draw() override;

    //今回に限っては始点と終点で十分
    void SetPoints(const Vector2& p0, const Vector2& p3);
    void SetColor(const XMFLOAT4& color);
};

