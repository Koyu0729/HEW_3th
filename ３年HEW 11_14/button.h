#pragma once

#include "gameObject.h"
#include <functional>
#include "moji.h"

//wchar_tÇ…ïœÇ¶ÇÈ
// 
//https://animate-club.com/button/css_btn_animation/

class Button : public GameObject2D
{
private:
    ID3D11Buffer* m_VertexBuffer;

    ID3D11InputLayout* m_VertexLayout = nullptr;
    ID3D11VertexShader* m_VertexShader = nullptr;
    ID3D11PixelShader* m_PixelShader = nullptr;

    ID3D11ShaderResourceView* m_Texture = nullptr;

    XMFLOAT4 m_Color = { 1,1,1,1 };

    bool m_Clicked = false;
    std::function<void()> m_OnClick;   // Å© âüÇ≥ÇÍÇΩÇ∆Ç´ÇÃèàóù

    FontData m_Font;
public:
    void Init()override;
    void Uninit() override;
    void Update() override;
    void Draw() override;

    bool IsClicked() const { return m_Clicked; }

    void SetOnClick(std::function<void()> onClick) { m_OnClick = onClick; }

    Button* SetTexRule(
        Vector2 position = Vector2(0.0f, 0.0f),
        Vector2 rotation = Vector2(0.0f, 0.0f),
        Vector2 scale = Vector2(0.0f, 0.0f),
        const char* FileName = "asset\\texture\\test.png"
    );

    bool Inside2D();
};
