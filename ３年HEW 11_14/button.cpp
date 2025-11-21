#include "main.h"
#include "button.h"
#include "renderer.h"
#include "texture.h"

void Button::Init()
{
    VERTEX_3D vertex[4];

    vertex[0].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

    vertex[1].Position = XMFLOAT3(1.0f, 0.0f, 0.0f);
    vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

    vertex[2].Position = XMFLOAT3(0.0f, 1.0f, 0.0f);
    vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

    vertex[3].Position = XMFLOAT3(1.0f, 1.0f, 0.0f);
    vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

    // 頂点バッファ生成
    D3D11_BUFFER_DESC bd{};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VERTEX_3D) * 4;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = vertex;

    Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

    // テクスチャ読み込み(念のため)
    m_Texture = Texture::Load("asset\\texture\\test.png");

    // シェーダー読み込み
    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
    Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

    m_Clicked = false;

    m_Position.x = 0.0f;
    m_Position.y = 0.0f;
    m_Scale.x    = 200.0f;
    m_Scale.y    = 100.0f;

    //文字初期設定
    m_Font.font = L"メイリオ";
    m_Font.fontSize = 32.0f;
    m_Font.Color = D2D1::ColorF(D2D1::ColorF::White);
    m_Font.shadowColor = D2D1::ColorF(D2D1::ColorF::Black);
    m_Font.shadowOffset = D2D1::Point2F(2, 2);
    //効果音

}

void Button::Uninit()
{
    m_VertexBuffer->Release();

    m_VertexLayout->Release();
    m_VertexShader->Release();
    m_PixelShader->Release();
}

void Button::Update()
{
    POINT pos = GetMousePos(); // 現在のマウス位置

    static bool prevLeft = false;

    if (Inside2D() && g_MouseLeft)
    {
        m_Clicked = true;
        if (m_OnClick) m_OnClick();
    }
    else
    {
        m_Clicked = false;
    }

   // printf("inside=%d g_MouseLeft=%d\n", inside, g_MouseLeft);
}

void Button::Draw()
{
    // 入力レイアウト設定
    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

    // シェーダ設定
    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

    // マトリクス設定
    Renderer::SetWorldViewProjection2D();

    // マトリクスの追加設定
    XMMATRIX world, scale, rot, trans;
    scale = XMMatrixScaling(m_Scale.x, m_Scale.y, 0.0f);
    trans = XMMatrixTranslation(m_Position.x, m_Position.y, 0.0f);

    world = scale * trans;

    Renderer::SetWorldMatrix(world);

    // マテリアル設定
    MATERIAL material{};
    material.Diffuse = m_Color;
    //material.Diffuse = XMFLOAT4(0.2, 0.2,0.2,1);
    material.TextureEnable = true;
    Renderer::SetMaterial(material);

    // 頂点バッファ設定
    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;
    Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

    // テクスチャ設定
    Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
    // プリミティブトポロジ設定
    Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    // ポリゴン描画
    Renderer::GetDeviceContext()->Draw(4, 0);

    //文字
    Vector2 buttonPos = GetPosition();
    Vector2 buttonScale = GetScale();

    D2D1_POINT_2F textPos = D2D1::Point2F(
        buttonPos.x + buttonScale.x * 0.2f, // 中央より少し右
        buttonPos.y + buttonScale.y * 0.3f  // 中央より少し下
    );

    float nameLength = (float)GetName().size();

    m_Font.fontSize = 32.0f * (1.0f - (nameLength * 0.05f));

    if (m_Font.fontSize < 24.0f)  m_Font.fontSize = 24.0f; // 最小サイズ制限
    if (m_Font.fontSize > 48.0f)  m_Font.fontSize = 48.0f;

    Moji::Instance().SetFont(m_Font);
    Moji::Instance().Draw(
        GetName(),
        textPos,
        1.0f,
        D2D1_DRAW_TEXT_OPTIONS_NONE,
        false
    );
}

Button* Button::SetTexRule(
    Vector2 position,
    Vector2 rotation,
    Vector2 scale,
    const char* FileName)
{
    m_Position = position;
    m_Rotation = rotation;
    m_Scale = scale;
    m_Texture = Texture::Load(FileName);

    return this;
}

bool Button::Inside2D()
{
    POINT pos = GetMousePos();
    Vector2 mouse((float)pos.x, (float)pos.y);

    Vector2 Pos = GetPosition();
    Vector2 Size = GetScale();

    return (mouse.x >= Pos.x && mouse.x <= Pos.x + Size.x &&
        mouse.y >= Pos.y && mouse.y <= Pos.y + Size.y);
}