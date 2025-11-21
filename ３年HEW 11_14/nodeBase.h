//内部処理だけを担当するノードの基底クラス
//ver1.1　変更点：前後の接続を記憶するようにした
#pragma once
#include <vector>
#include <string>
#include "nodeList.h"

#define ID_NULL (-1)
#define ID_NEXT_NULL (-1)

class Player;

class NodeBase
{
protected:
    NodeCategory m_Category;        //派生ノードのカテゴリ
    NodeType m_Type;                //カテゴリの中の種類
    std::wstring m_Name;             //個別の名前

    int m_ID = ID_NULL;          //生成されたときの番号

    //ノード別に
    float frame = 0.0f;

    //プレイヤーの情報（ポインタ）
    Player* m_GameChara = nullptr;
public:
    //派生ノードに継承させていき段階的にカテゴライズしていく
    NodeBase(int id, NodeCategory category, NodeType type)
        : m_ID(id), m_Category(category), m_Type(type)
    {
        const auto& list = NodeTable.at(category);

        for (auto& info : list)
        {
            if (info.type == m_Type)
            {
                m_Name = info.name;
                break;
            }
        }
    }

    // 派生で個別の処理を書く関数
    virtual NodeBase* Execute() = 0;

    // ノード情報取得
    NodeCategory GetCategory() const { return m_Category; }
    NodeType     GetType()     const { return m_Type; }
    const std::wstring& GetName() const { return m_Name; }
    int GetID() const { return m_ID; }

    //thisノードの次のポインターを格納する(setter)
    virtual void SetNext(NodeBase* next, Port port = Port::OutDef) = 0;

    //thisノードの次のポインターを取得する(getter)
    virtual NodeBase* GetNext() const = 0;

    void SetPlayer(Player* player) { m_GameChara = player; }
    Player* GetPlayer() { return m_GameChara; }
};

