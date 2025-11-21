#pragma once
#include "nodeBase.h"

// 条件分岐系の共通クラス
// NodeManager の接続情報を利用して、
// True/False のどちらに進むかだけを返す。
class BranchBase : public NodeBase
{
protected:
    bool m_BranchResult = false;
    //ポインターを持っている方が実行時に早い
    NodeBase* m_TrueNext = nullptr;
    NodeBase* m_FalseNext = nullptr;

public:
    BranchBase(int id, NodeType type)
        : NodeBase(id, NodeCategory::If, type)
    {
    }

    // 条件評価（派生ノードで実装）
    virtual bool Branch() = 0;

    // 実行（評価して結果を保存）
    NodeBase* Execute() override
    {
        m_BranchResult = Branch();

        // 判定ログ
        printf("Branch 判定結果: %s\n", m_BranchResult ? "True" : "False");

        if (m_BranchResult && m_TrueNext)
        {
            return m_TrueNext;
        }
        else if (!m_BranchResult && m_FalseNext)
        {
            return m_FalseNext;
        }
        else
        {
            return nullptr;
        }
    };

    bool GetConditionResult() const { return m_BranchResult; }

    //げったー・せったー
    void SetNext(NodeBase* next, Port from) override
    {
        if (from == Port::OutTrue)
        {
            m_TrueNext = next;
        }
        else if (from == Port::OutFalse)
        {
            m_FalseNext = next;
        }
        else
        {
            printf("エラー");
        }
    }

    NodeBase* GetNext() const override
    {
        //三項演算（リザルトがtrueなら左、falseなら右）
        //見る人がいたとき困るから念のため記述
        return m_BranchResult ? m_TrueNext : m_FalseNext;
    }
};
