//ノードの描画だけの担当（ノードクリエイトでノードの内部処理と混ぜる）
//ver1.1　変更点：触ると選択状態になる。カテゴリーごとに色を追加。可読性の向上
#include "main.h"
#include "nodeView.h"
#include "polyTex.h"
#include "manager.h"
#include "scene.h"

void NodeView::Init(NodeBase* node)
{
    m_Node = node;
    m_Category = node->GetCategory();

    POINT mouse = GetMousePos();

    switch (m_Category)
    {
    case NodeCategory::Start:
        m_BaseColor = D2D1::ColorF(D2D1::ColorF::Gold);
        break;
    case NodeCategory::If:
        m_BaseColor = D2D1::ColorF(D2D1::ColorF::SkyBlue);
        break;
    case NodeCategory::Attack:
        m_BaseColor = D2D1::ColorF(D2D1::ColorF::Tomato);
        break;
    case NodeCategory::Action:
        m_BaseColor = D2D1::ColorF(D2D1::ColorF::MediumSeaGreen);
        break;
    case NodeCategory::Movement:
        m_BaseColor = D2D1::ColorF(D2D1::ColorF::YellowGreen);
        break;
    default:
        m_BaseColor = D2D1::ColorF(D2D1::ColorF::Gray);
        break;
    }

    m_CurrentColor = m_BaseColor;

    nodeRect = Manager::GetScene()->AddGameObject2D<PolyTex>(3);
    nodeRect->SetTexRule(
        Vector2((float)mouse.x, (float)mouse.y),
        Vector2(0.0f, 0.0f),
        Vector2(RECT_WIDTH, RECT_HEIGHT),
        "asset\\texture\\node\\rectangle.png"
    );
    nodeRect->SetName(L"NodeRect");

    Vector2 nodePos = nodeRect->GetPosition();
    Vector2 nodeScale = nodeRect->GetScale();

    Vector2 center(
        nodePos.x + nodeScale.x * 0.5f,
        nodePos.y + nodeScale.y * 0.38f
    );

    if (m_Category != NodeCategory::Start)
    {
        CreatePort(
            nodeIn,
            L"NodeIn",
            center.x - nodeScale.x * IN_PORT_POSX,
            center.y
        );
    }
    
    if (m_Category == NodeCategory::If)
    {
        CreatePort(
            nodeOutTrue,
            L"NodeOutTrue",
            center.x + nodeScale.x * OUT_PORT_POSX,
            center.y - nodeScale.y * OUT_IF_PORT_POSY,
            "asset\\texture\\node\\true.png"
        );

        CreatePort(
            nodeOutFalse,
            L"NodeOutFalse",
            center.x + nodeScale.x * OUT_PORT_POSX,
            center.y + nodeScale.y * OUT_IF_PORT_POSY,
            "asset\\texture\\node\\false.png"
        );
    }
    else
    {
        CreatePort(
            nodeOutDef,
            L"NodeOut",
            center.x - nodeScale.x * IN_PORT_POSX,
            center.y
        );
    }

    //文字初期設定
    m_Font.font = L"メイリオ";
    m_Font.fontSize = 32.0f;
    m_Font.Color = D2D1::ColorF(D2D1::ColorF::White);
    m_Font.shadowColor = D2D1::ColorF(D2D1::ColorF::Black);
    m_Font.shadowOffset = D2D1::Point2F(2, 2);
}

void NodeView::Uninit()
{
    // ノード矩形の破棄
    if (nodeRect)
    {
        nodeRect->SetDestroy();
        nodeRect = nullptr;
    }

    // ポートの破棄
    if (nodeIn)
    {
        nodeIn->SetDestroy();
        nodeIn = nullptr;
    }

    if (nodeOutDef)
    {
        nodeOutDef->SetDestroy();
        nodeOutDef = nullptr;
    }

    if (nodeOutTrue)
    {
        nodeOutTrue->SetDestroy();
        nodeOutTrue = nullptr;
    }

    if (nodeOutFalse)
    {
        nodeOutFalse->SetDestroy();
        nodeOutFalse = nullptr;
    }

    // 状態リセット
    m_IsDragging = false;
    m_Node = nullptr;
}

void NodeView::Update()
{
    POINT pos = GetMousePos();
    Vector2 mousePos((float)pos.x, (float)pos.y);
    
    Vector2 rectPos = nodeRect->GetPosition();
    Vector2 rectSize = nodeRect->GetScale();

    if (nodeRect->Inside2D() && g_MouseLeftDown && !m_IsDragging)
    {
        m_IsDragging = true;
        m_DragOffset = mousePos - rectPos;
    }

    if (g_MouseLeftUp)
    {
        m_IsDragging = false;
    }

    if (m_IsDragging)
    {
        nodeRect->SetPosition(mousePos - m_DragOffset);
    }

    Vector2 center(
        rectPos.x + rectSize.x * 0.5f,
        rectPos.y + rectSize.y * 0.38f
    );

    if (m_Category != NodeCategory::Start && nodeIn)
    {
        nodeIn->SetPosition(Vector2(center.x - rectSize.x * IN_PORT_POSX, center.y));
    }

    if (m_Category == NodeCategory::If)
    {
        nodeOutTrue->SetPosition(Vector2(center.x + rectSize.x * OUT_PORT_POSX, center.y - rectSize.y * OUT_IF_PORT_POSY));
        
        nodeOutFalse->SetPosition(Vector2(center.x + rectSize.x * OUT_PORT_POSX, center.y + rectSize.y * OUT_IF_PORT_POSY));
    }
    else if (nodeOutDef)
    {
        nodeOutDef->SetPosition(Vector2(center.x + rectSize.x * OUT_PORT_POSX, center.y));
    } 
    
    if (nodeRect)
    {
        XMFLOAT4 color = XMFLOAT4(m_CurrentColor.r, m_CurrentColor.g, m_CurrentColor.b, m_CurrentColor.a);
        nodeRect->SetColor(color); //これで現在色を適用
    }
}

void NodeView::Draw()
{
    Vector2 rectPos = nodeRect->GetPosition();
    Vector2 rectScale = nodeRect->GetScale();

    D2D1_POINT_2F textPos = D2D1::Point2F(
        rectPos.x + rectScale.x * 0.2f, // 中央より少し右
        rectPos.y + rectScale.y * 0.3f  // 中央より少し下
    );
    float nameLength = (float)GetNode()->GetName().size();

    m_Font.fontSize = 32.0f * (1.0f - (nameLength * 0.05f));

    if (m_Font.fontSize < 16.0f)  m_Font.fontSize = 16.0f; // 最小サイズ制限
    if (m_Font.fontSize > 48.0f)  m_Font.fontSize = 48.0f;

    Moji::Instance().SetFont(m_Font);
    Moji::Instance().Draw(
        GetNode()->GetName(),
        textPos,
        m_zoom,
        D2D1_DRAW_TEXT_OPTIONS_NONE,
        false
    );

}

//接続用

//ノードエディターにポートの当たり判定を渡す
bool NodeView::InsidePort(Port port)
{
    POINT pos = GetMousePos();
    Vector2 mouse((float)pos.x, (float)pos.y);

    PolyTex* target = nullptr;
    switch (port)
    {
    case Port::In:
        target = nodeIn;
        break;

    case Port::OutDef:
        target = nodeOutDef;
        break;

    case Port::OutTrue:
        target = nodeOutTrue;
        break;

    case Port::OutFalse:
        target = nodeOutFalse;
        break;
    }

    //絶対に一回は入ってしまう。
    //考察：gameObjectがInitされるタイミングで呼べれば解決するけど、処理順的に今のままだと無理？たぶん...。
    if (!target) return false;

    Vector2 targetPos = target->GetPosition();
    Vector2 targetSize = target->GetScale();

    return (mouse.x >= targetPos.x && mouse.x <= targetPos.x + targetSize.x &&
        mouse.y >= targetPos.y && mouse.y <= targetPos.y + targetSize.y);
}

//ポートの中心から線を出したいから
Vector2 NodeView::GetPortCenter(Port port)
{
    PolyTex* target = nullptr;
    switch (port)
    {
    case Port::In:
        target = nodeIn;
        break;

    case Port::OutDef:
        target = nodeOutDef;
        break;

    case Port::OutTrue:
        target = nodeOutTrue;
        break;

    case Port::OutFalse:
        target = nodeOutFalse;
        break;
    }

    //絶対に一回は入ってしまう。
    //考察：gameObjectがInitされるタイミングで呼べれば解決するけど、処理順的に今のままだと無理？たぶん...。
    if (!target) {
        return Vector2(0, 0);
    }

    Vector2 p = target->GetPosition();
    Vector2 s = target->GetScale();
    return Vector2(p.x + s.x * 0.5f, p.y + s.y * 0.5f);
}

void NodeView::SetHighlighted(bool highlight)
{
    if (highlight)
        m_CurrentColor = m_HighlightColor;
    else
        m_CurrentColor = m_BaseColor;

    if (nodeRect)
    {
        XMFLOAT4 color(
            m_CurrentColor.r,
            m_CurrentColor.g,
            m_CurrentColor.b,
            m_CurrentColor.a
        );
        nodeRect->SetColor(color);
    }
}

void NodeView::CreatePort(PolyTex*& port, const std::wstring& name, float x, float y, const char* tex)
{
    port = Manager::GetScene()->AddGameObject2D<PolyTex>(3);
    port->SetTexRule(
        Vector2(x, y),
        Vector2(0.0f, 0.0f),
        Vector2(20.0f, 20.0f),
        tex
    );
    port->SetName(name);
}