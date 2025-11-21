//右クリックで新しいノードを作るときに出てくる選択式リスト
#pragma once

#include "scene.h"
#include "moji.h"
#include "nodeBase.h"
#include "polyTex.h"
#include "textObject.h"

class NodeCreateList : public GameObject2D
{
private:
    //枠
	PolyTex* m_ListFront = nullptr;
    //裏
    PolyTex* m_ListBack = nullptr;

    //文字タイプの数だけ確保
    FontData m_CategoryFont[(int)NodeCategory::Category_MAX];
    TextObject* m_CategoryTextObject[(int)NodeCategory::Category_MAX];

	FontData m_Font[(int)NodeType::Type_MAX];
    TextObject* m_textObject[(int)NodeType::Type_MAX];

	float m_ScrollOffset = 0.0f;

	bool m_Visible = false;
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

    //右クリック位置に表示するため
    void SetPosition(const Vector2& pos)
    {
        if (m_ListFront)
            m_ListFront->SetPosition(pos);
        if (m_ListBack)
            m_ListBack->SetPosition(pos);
    }
};