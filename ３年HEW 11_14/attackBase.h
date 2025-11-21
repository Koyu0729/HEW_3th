#pragma once
#include "nodeBase.h"

class AttackBase : public NodeBase
{
protected:
    float m_Damage = 10.0f;
    
    //ポインターを持っている方が実行時に早い
    NodeBase* m_NextOutput = nullptr;
public:
    AttackBase(int id, NodeType type)
        : NodeBase(id, NodeCategory::Attack, type)
    {
    }

    //げったー・せったー
    void SetNext(NodeBase* next, Port port = Port::OutDef) override
    {
        m_NextOutput = next;
    }
    NodeBase* GetNext() const override { return m_NextOutput; }
};
