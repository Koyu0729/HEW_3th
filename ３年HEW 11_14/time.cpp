#include "main.h"
#include "renderer.h"
#include "time.h"
#include "texture.h"

void Time::Init()
{
    // 頂点バッファ準備（数字ポリゴン用）
    VERTEX_3D vertex[4]{};

    vertex[0].Position = XMFLOAT3(-1.0f, 1.0f, 0.0f);
    vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

    vertex[1].Position = XMFLOAT3(1.0f, 1.0f, 0.0f);
    vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

    vertex[2].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
    vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

    vertex[3].Position = XMFLOAT3(1.0f, -1.0f, 0.0f);
    vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

    D3D11_BUFFER_DESC bd{};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(VERTEX_3D) * 4;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = vertex;

    Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

    // テクスチャ読み込み (数字+小数点をまとめたスプライト)
    m_Texture = Texture::Load("asset\\texture\\time.png");

    // シェーダー
    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
    Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

    // 初期状態はカウントダウン
    m_Time = 3.0f; // 3秒カウントダウン
    m_Mode = TimeMode::Countdown;
    m_Value = 0;
}

void Time::Uninit()
{
    m_Texture->Release(); m_Texture = nullptr;
    m_VertexBuffer->Release(); m_VertexBuffer = nullptr;
    m_VertexLayout->Release(); m_VertexLayout = nullptr;
    m_VertexShader->Release(); m_VertexShader = nullptr;
    m_PixelShader->Release(); m_PixelShader = nullptr;
}

void Time::Update()
{
    float dt = 1.0f / 60.0f;

    if (m_Mode == TimeMode::Countdown)
    {
        m_Time -= dt;
        if (m_Time <= 0.0f)
        {
            m_Time = 0.0f;
            m_Mode = TimeMode::Countup; // カウントアップに移行
        }
    }
    else if (m_Mode == TimeMode::Countup)
    {
        m_Time += dt;
    }
}


void Time::Draw()
{
    // 共通設定
    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);
    Renderer::SetWorldViewProjection2D();

    MATERIAL material{};
    material.Diffuse = { 1,1,1,1 };
    material.TextureEnable = true;
    Renderer::SetMaterial(material);

    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;
    Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
    Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

    Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    if (m_Mode == TimeMode::Countdown)
    {
        int count = static_cast<int>(ceilf(m_Time));
        if (count > 0)
        {
            float digitWidth = 50.0f;
            float startX = (SCREEN_WIDTH - digitWidth) / 2.0f; // 横中央
            float startY = 50.0f;                              // 上固定
            DrawDigit(count, startX, startY);
        }
    }
    else if (m_Mode == TimeMode::Countup)
    {
        // 経過時間を文字列化 (小数1桁)
        char buffer[16];
        sprintf_s(buffer, "%.1f", m_Time);

        int len = (int)strlen(buffer);
        float digitWidth = 30.0f;
        float totalWidth = len * digitWidth;

        float startX = (SCREEN_WIDTH - totalWidth) / 2.0f; // 横中央
        float startY = 50.0f;                              // 上固定

        for (int i = 0; i < len; i++)
        {
            int index;
            if (buffer[i] == '.')
                index = 13;   // 小数点のインデックス
            else
                index = buffer[i] - '0';

            DrawDigit(index, startX + i * digitWidth, startY);
        }
    }
}

void Time::DrawDigit(int num, float x, float y)
{
    float tw = 1.0f / 5;
    float th = 1.0f / 5;
    float tx = (num % 5) * tw;
    float ty = (num / 5) * th;

    D3D11_MAPPED_SUBRESOURCE msr;
    Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

    VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

    float w = 50.0f, h = 50.0f;

    vertex[0].Position = XMFLOAT3(x, y, 0.0f);
    vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
    vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[0].TexCoord = XMFLOAT2(tx, ty);

    vertex[1].Position = XMFLOAT3(x + w, y, 0.0f);
    vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
    vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[1].TexCoord = XMFLOAT2(tx + tw, ty);

    vertex[2].Position = XMFLOAT3(x, y + h, 0.0f);
    vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
    vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[2].TexCoord = XMFLOAT2(tx, ty + th);

    vertex[3].Position = XMFLOAT3(x + w, y + h, 0.0f);
    vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
    vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[3].TexCoord = XMFLOAT2(tx + tw, ty + th);

    Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

    Renderer::GetDeviceContext()->Draw(4, 0);
}
