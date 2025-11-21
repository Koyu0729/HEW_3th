//表示（ノードの見た目だけ用）
#pragma once
#include "gameObject.h"
#include "nodeBase.h"
#include "moji.h"

#define IN_PORT_POSX     (0.65f)
#define OUT_PORT_POSX    (0.55f)
#define OUT_IF_PORT_POSY  (0.2f)
#define RECT_WIDTH        (200.0f)
#define RECT_HEIGHT       (100.0f)

class PolyTex;

class NodeView : public GameObject2D
{
private:
#pragma region 描画用

    PolyTex* nodeRect = nullptr;
    PolyTex* nodeIn = nullptr;
    PolyTex* nodeOutDef = nullptr;
    PolyTex* nodeOutTrue = nullptr;
    PolyTex* nodeOutFalse = nullptr;

    FontData m_Font;

    D2D1::ColorF m_BaseColor = D2D1::ColorF(D2D1::ColorF::Gray);
    D2D1::ColorF m_HighlightColor = D2D1::ColorF(D2D1::ColorF::White);
    D2D1::ColorF m_CurrentColor = m_BaseColor;

#pragma endregion

#pragma region マウスのやつ

    //マウスが全体移動だったのでメンバ変数に移動
    bool m_IsDragging = false;
    Vector2 m_DragOffset;

    //拡大・縮小用
    float m_zoom = 1.0f;
    

#pragma endregion

    NodeBase* m_Node = nullptr;
    NodeCategory m_Category = NodeCategory::Category_Null;
public:
    void Init(NodeBase* node);

    void Init()override {};
    void Uninit() override;
    void Update() override;
    void Draw()   override;
    
    //ポート描画用
    void CreatePort(
        PolyTex*& port,
        const std::wstring& name,
        float x = 0, float y = 0,
        const char* tex = "asset\\texture\\node\\null.png"
    );

    void ZoomNodeEditor(float zoom) { m_zoom = zoom; };

    //ノードエディター用

    //ノードの本体
    PolyTex* GetRect() const { return nodeRect; }

    //ノードの当たり判定
    bool InsidePort(Port port);

    //ポートの中央
    Vector2 GetPortCenter(Port port);

    //格納してる内部処理
    NodeBase* GetNode() const { return m_Node; }

    void SetHighlighted(bool on);
};
