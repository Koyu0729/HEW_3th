#pragma once
#include "nodeBase.h"

class MoveBase : public NodeBase
{
protected:
    //ポインターを持っている方が実行時に早い
    NodeBase* m_NextOutput = nullptr;
public:
    MoveBase(int id, NodeType type)
        : NodeBase(id, NodeCategory::Movement, type)
    {
    }

    //げったー・せったー
    void SetNext(NodeBase* next, Port port = Port::OutDef) override
    {
        m_NextOutput = next;
    }
    NodeBase* GetNext() const override { return m_NextOutput; }
};
