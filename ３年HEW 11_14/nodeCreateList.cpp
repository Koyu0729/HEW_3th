//ノードエディターの最初に呼ぶ（右クリックで表示）
#include "main.h"
#include "nodeCreateList.h"
#include "polyTex.h"
#include "manager.h"
#include "scene.h"

PolyTex* m_ListFront;
PolyTex* m_ListBack;

void NodeCreateList::Init()
{
    //文字初期設定
    for (int i = 0; i < (int)NodeCategory::Category_MAX; i++)
    {
        m_CategoryFont[i].font = L"メイリオ";
        m_CategoryFont[i].fontSize = 32.0f;
        m_CategoryFont[i].Color = D2D1::ColorF(D2D1::ColorF::White);
        m_CategoryFont[i].shadowColor = D2D1::ColorF(D2D1::ColorF::Black);
        m_CategoryFont[i].shadowOffset = D2D1::Point2F(2, 2);

        m_CategoryTextObject[i] = Manager::GetScene()->AddGameObject2D<TextObject>(8);
        m_CategoryTextObject[i]->SetFont(m_CategoryFont[i]);
    }

    for (int i = 0; i < (int)NodeType::Type_MAX; i++)
    {
        m_Font[i].font = L"メイリオ";
        m_Font[i].fontSize = 24.0f;
        m_Font[i].Color = D2D1::ColorF(D2D1::ColorF::White);
        m_Font[i].shadowColor = D2D1::ColorF(D2D1::ColorF::Black);
        m_Font[i].shadowOffset = D2D1::Point2F(2, 2);

        m_textObject[i] = Manager::GetScene()->AddGameObject2D<TextObject>(8);
        m_textObject[i]->SetFont(m_Font[i]);
    }

    POINT mouse = GetMousePos();

    //リスト（今は、テクスチャ）を生成
    m_ListFront = Manager::GetScene()->AddGameObject2D<PolyTex>(9);
    m_ListFront->SetTexRule(
        Vector2(0.0f, 0.0f),
        Vector2(0.0f, 0.0f),
        Vector2(240.0f, SCREEN_HEIGHT),
        "asset\\texture\\listFront.png"
    );

    m_ListBack = Manager::GetScene()->AddGameObject2D<PolyTex>(7);
    m_ListBack->SetTexRule(
        Vector2(0.0f, 0.0f),
        Vector2(0.0f, 0.0f),
        Vector2(240.0f, SCREEN_HEIGHT),
        "asset\\texture\\listBack.png"
    );

    m_ListFront->SetColor(XMFLOAT4(1, 1, 1, 1.0f));

    m_ListBack->SetColor(XMFLOAT4(1, 1, 0, 0.0f));


    m_ScrollOffset = 0.0f;
}

void NodeCreateList::Uninit()
{
    if (m_ListFront)
    {
        m_ListFront->SetDestroy();
        m_ListFront = nullptr;
    }

    if (m_ListBack)
    {
        m_ListBack->SetDestroy();
        m_ListBack = nullptr;
    }

    for (int i = 0; i < (int)NodeCategory::Category_MAX; i++)
    {
        m_CategoryTextObject[i]->SetDestroy();
        m_CategoryTextObject[i] = nullptr;
    }

    for (int i = 0; i < (int)NodeType::Type_MAX; i++)
    {
        m_textObject[i]->SetDestroy();
        m_textObject[i] = nullptr;
    }
}

void NodeCreateList::Update()
{
    if (!m_ListFront) return;
    if (!m_ListBack) return;

    // マウス位置取得
    POINT pos = GetMousePos();
    Vector2 mouse((float)pos.x, (float)pos.y);

    Vector2 listPos = m_ListBack->GetPosition();
    Vector2 listSize = m_ListBack->GetScale();

    bool inside =
        (mouse.x >= listPos.x && mouse.x <= listPos.x + listSize.x &&
            mouse.y >= listPos.y && mouse.y <= listPos.y + listSize.y);

    // マウスホイールスクロール（画像の上だけ反応）
    if (inside)
    {
        float wheel = GetMouseWheel();
        m_ScrollOffset += wheel * 10.0f;

        //// 範囲制限（ノードの数）
        //float maxScroll = (int)NodeType::Type_MAX * 40.0f - listSize.y + 20.0f;
        //if (m_ScrollOffset < 0) m_ScrollOffset = 0;
        //if (m_ScrollOffset > maxScroll) m_ScrollOffset = maxScroll;
    }
}

void NodeCreateList::Draw()
{
    if (!m_ListFront) return;
    if (!m_ListBack) return;

    Vector2 listPos = m_ListBack->GetPosition();

    float baseY = listPos.y + 20.0f - m_ScrollOffset;
    float lineHeight = 40.0f;

    for (int i = 0; i < (int)NodeType::Type_MAX; ++i)
    {
        float y = baseY + i * lineHeight;

        // 表示範囲外なら描画しない（軽量化）
        if (y < listPos.y || y > listPos.y + m_ListBack->GetScale().y)
            continue;

        m_textObject[i]->SetPosition(Vector2(listPos.x + 20.0f, y));

        const wchar_t* name = NodeTable.at(NodeCategory::Category_MAX)[i].name;
        m_textObject[i]->SetText(name);
    }
}