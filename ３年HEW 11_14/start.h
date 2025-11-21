#pragma once
#include "nodeBase.h"

class Start : public NodeBase
{
protected:
    NodeBase* m_NextOutput = nullptr;
public:
    Start(int id)
        : NodeBase(id, NodeCategory::Start, NodeType::Start)
    {
    }

    NodeBase* Execute() override;

    //Ç∞Ç¡ÇΩÅ[ÅEÇπÇ¡ÇΩÅ[
    void SetNext(NodeBase* next, Port port = Port::OutDef) override
    {
        m_NextOutput = next;
    }
    NodeBase* GetNext() const override { return m_NextOutput; }
};
